
#include "pch.h"

#include "globals.h"
#include "freecam.h"
#include "freefly.h"
#include "math.h"

BYTE stored[5]{};

Utils::Vector3 freecam::GetStepVector(Utils::Vector3 player_angles, float amount){
    Utils::Vector3 step{};

    float pitch = player_angles.x;
    float yaw = player_angles.y;

    step.x = cosf(Utils::DegreeToRadian(yaw)) * amount * cosf(Utils::DegreeToRadian(pitch));
    step.y = sinf(Utils::DegreeToRadian(yaw)) * amount * cosf(Utils::DegreeToRadian(pitch));
    step.z = sinf(Utils::DegreeToRadian(pitch)) * amount;
 
    return step;
}

bool freecam::EnableFreeCam(){
    DWORD cameraFix = (DWORD)(globals::hwdllBase + 0x2052B);

    DWORD oldProtect{};
    VirtualProtect((LPVOID)cameraFix, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    for(int i = 0; i < 5; i++){
        stored[i] = *(BYTE*)(cameraFix + i);
        *(BYTE*)(cameraFix + i) = 0x90;
    }

    VirtualProtect((LPVOID)cameraFix, 5, oldProtect, &oldProtect);

    return true;
}

bool freecam::DisableFreeCam(){
    DWORD cameraFix = (DWORD)(globals::hwdllBase + 0x2052B);

    DWORD oldProtect{};
    VirtualProtect((LPVOID)cameraFix, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    for(int i = 0; i < 5; i++){
        *(BYTE*)(cameraFix + i) = stored[i];
        stored[i] = 0;
    }

    VirtualProtect((LPVOID)cameraFix, 5, oldProtect, &oldProtect);

    return false;
}

void freecam::FreecamStep(float stepAmount, int dir){
    globals::PlayerCamera cam{};

    Utils::Vector3 player_angles{};

    if(dir == freecam::DIR::LEFT){
        player_angles = {0.0f, *cam.yaw + 90.0f, *cam.roll};
    } else if(dir == freecam::DIR::RIGHT){
        player_angles = {0.0f, *cam.yaw + 270.0f, *cam.roll};
    } else {
        player_angles = {*cam.pitch, *cam.yaw, *cam.roll};
    }

    Utils::Vector3 step = freefly::GetStepVector(player_angles, 5.0f);

    if(dir == freecam::DIR::BACK){
        *cam.pos_x -= step.x;
        *cam.pos_y -= step.y;
        *cam.pos_z -= step.z;
    } else {
        *cam.pos_x += step.x;
        *cam.pos_y += step.y;
        *cam.pos_z += step.z;
    }
}