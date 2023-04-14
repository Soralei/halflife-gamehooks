// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <iostream>
#include "Hookz.h"
#include "freefly.h"
#include "freecam.h"
#include "globals.h"
#include "ESP.h"

DWORD globals::hwdllBase{};

DWORD* globals::playerBase{};
DWORD* globals::entityList{};
float* globals::MVP_Matrix{};

bool globals::freeflyEnabled = false;
bool globals::freecamEnabled = false;
bool globals::espEnabled = false;
bool globals::espDebugEnabled = false;

DWORD OpenGLReturnAddress{};
auto doEsp = ESP::DoESP;
void __declspec(naked) OpenGLRedirect()
{
    __asm{
        PUSHFD
        PUSHAD
        CALL doEsp // Hijack.
        POPAD
        POPFD
        PUSH EBX // Recreate the original instructions to avoid corruption.
        PUSH ESI
        PUSH EDI
        MOV EDI, [EBP + 0x08]
        JMP [OpenGLReturnAddress] // Return the flow to OpenGL.
    }
}

BOOL WINAPI MainThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    globals::hwdllBase = (DWORD)GetModuleHandle(L"hw.dll");
    if(globals::hwdllBase == NULL){
        std::cout << "Something went wrong with getting the address for hw.dll" << '\n';
    }

    globals::playerBase = (DWORD*)(globals::hwdllBase + 0x7F6304);
    globals::MVP_Matrix = (float*)(globals::hwdllBase + 0xEC9780);
    globals::entityList = (DWORD*)(globals::hwdllBase + 0x843D60);

    // OpenGL hooking.
    Hookz* OpenGLHook = NULL;

    // Freefly related nopping.
    Nop* xMovementNop = NULL;
    Nop* yMovementNop = NULL;
    Nop* zMovementNop = NULL;
    Nop* zVelocityNop = NULL;

    DWORD OpenGL_adr = (DWORD)GetModuleHandle(L"OPENGL32.dll");
    DWORD wglsb_hook_adr{}; // wglSwapBuffers hook address.
    if(OpenGL_adr == NULL){
        std::cout << "Something went wrong with getting the address to OPENGL32.dll" << '\n';
    } else {
        wglsb_hook_adr = OpenGL_adr + 0x40B3B;
    }

    std::cout << "DLL successfully injected. Hello from within! Press VK_NUMPAD0 to eject." << '\n';

    while(true){
        // Exit.
        if(GetAsyncKeyState(VK_NUMPAD0) & 0x8000){
            std::cout << "NUMPAD0 pressed. Ejecting." << '\n';
            break;
        }

        // Shift key activation.
        if(GetAsyncKeyState(VK_SHIFT) & 0x8000){

            SHORT isC = GetAsyncKeyState(0x43);
            SHORT isX = GetAsyncKeyState(0x58);
            SHORT isZ = GetAsyncKeyState(0x5A);
            SHORT isQ = GetAsyncKeyState(0x51);

            // Used for debugging.
            if(isQ){
                globals::Player* ply = *(globals::Player**)globals::playerBase;
                std::cout << "pos_true.x: " << ply->pos_true.x << '\n';
                std::cout << "pos_true.y: " << ply->pos_true.y << '\n';
                std::cout << "pos_true.z: " << ply->pos_true.z << '\n';
                std::cout << "health: " << ply->health << '\n';
                std::cout << "type: " << ply->type << '\n';
                std::cout << "feetpos: " << ply->pos_feet.z << '\n';
                std::cout << "headpos: " << ply->pos_head.z << '\n';

                Sleep(5);
            }

            // Toggle freefly.
            if(isX){
                // Make sure freecam isn't enabled at the same time.
                if(globals::freecamEnabled){
                    globals::freecamEnabled = false;
                    freecam::DisableFreeCam();
                }
                globals::freeflyEnabled = !globals::freeflyEnabled;

                if(globals::freeflyEnabled){

                    if(xMovementNop == NULL){
                        xMovementNop = new Nop(globals::hwdllBase + 0xA8567, 6);
                    }
                    xMovementNop->Enable();

                    if(yMovementNop == NULL){
                        yMovementNop = new Nop(globals::hwdllBase + 0xA857B, 6);
                    }
                    yMovementNop->Enable();

                    if(zMovementNop == NULL){
                        zMovementNop = new Nop(globals::hwdllBase + 0xA858F, 6);
                    }
                    zMovementNop->Enable();

                    if(zVelocityNop == NULL){
                        zVelocityNop = new Nop(globals::hwdllBase + 0xA85CB, 6);
                    }
                    zVelocityNop->Enable();
                        
                } else {
                    xMovementNop->Disable();
                    zMovementNop->Disable();
                    yMovementNop->Disable();
                    zVelocityNop->Disable();
                }

                std::cout << "Freefly: " << std::boolalpha << globals::freeflyEnabled << std::noboolalpha << '\n';
            }

            // Toggle free.
            if(isZ){
                // Make sure freefly isn't used at the same time.
                if(globals::freeflyEnabled){
                    globals::freeflyEnabled = false;
                }

                globals::freecamEnabled = !globals::freecamEnabled;
                std::cout << "Freecam: " << std::boolalpha << globals::freecamEnabled << std::noboolalpha << '\n';

                if(globals::freecamEnabled){
                    freecam::EnableFreeCam();
                } else {
                    freecam::DisableFreeCam();
                }
            }

            // Toggle ESP.
            if(isC){
                if(!globals::espEnabled){
                    if(OpenGLHook == NULL){
                        // Hook OpenGL, if not already hooked.
                        OpenGLReturnAddress = wglsb_hook_adr + 5; // The return address for the redirect function.
                        OpenGLHook = new Hookz(wglsb_hook_adr, (DWORD)OpenGLRedirect, 6); // Define the hook.
                        OpenGLHook->Enable(); // Attempt to enable it.
                    }

                    if(OpenGLHook->IsActive()){
                        globals::espEnabled = true; // Enable ESP, but make sure that the hook is successful and active first.
                    }
                } else {
                    globals::espEnabled = false;
                }
                std::cout << "ESP: " << std::boolalpha << globals::espEnabled << std::noboolalpha << '\n';
            }

            Sleep(100);
        }

        // Numpad controls.
        if(GetAsyncKeyState(VK_NUMPAD8) & 0x8000){
            // Fly forwards.
            if(globals::freeflyEnabled){
                freefly::PlayerStep(5.0f);
                Sleep(5);
            } else if(globals::freecamEnabled){
                freecam::FreecamStep(5.0f);
                Sleep(5);
            }
        }

        if(GetAsyncKeyState(VK_NUMPAD4) & 0x8000){
            // Fly left.
            if(globals::freeflyEnabled){
                freefly::PlayerStep(5.0f, freefly::DIR::LEFT);
                Sleep(5);
            } else if(globals::freecamEnabled){
                freecam::FreecamStep(5.0f, freecam::DIR::LEFT);
                Sleep(5);
            }
        }

        if(GetAsyncKeyState(VK_NUMPAD6) & 0x8000){
            // Fly right.
            if(globals::freeflyEnabled){
                freefly::PlayerStep(5.0f, freefly::DIR::RIGHT);
                Sleep(5);
            } else if(globals::freecamEnabled){
                freecam::FreecamStep(5.0f, freecam::DIR::RIGHT);
                Sleep(5);
            }
        }

        if(GetAsyncKeyState(VK_NUMPAD5) & 0x8000){
            // Fly back.
            if(globals::freeflyEnabled){
                freefly::PlayerStep(5.0f, freefly::DIR::BACK);
                Sleep(5);
            } else if(globals::freecamEnabled){
                freecam::FreecamStep(5.0f, freecam::DIR::BACK);
                Sleep(5);
            }
        }

        if(GetAsyncKeyState(VK_DELETE) & 0x8000){
            if(globals::espEnabled){
                globals::espDebugEnabled = !globals::espDebugEnabled;
                Sleep(150);
            }
        }

        Sleep(5);
    }

    // Disable the OpenGLHook before shutting down, if it exists and is active.
    if(OpenGLHook != NULL && OpenGLHook->IsActive()){
        OpenGLHook->Disable();
    }

    if(f != NULL){
        fclose(f);
    }
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE threadHandle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
        if(threadHandle != NULL)
        {
            CloseHandle(threadHandle);
        }
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

