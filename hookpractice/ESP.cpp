#include "pch.h"

#include "ESP.h"

#include <iostream>
#include <string>

OGL::Font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;

void ESP::DoESP(){
    // Only draw if the ESP is enabled.
    if(!globals::espEnabled){
        return;
    }

    OGL::SetOrtho();

    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);

    if(globals::espDebugEnabled){
        // Draw a box at top left and top right, used when testing the viewport.
        OGL::DrawEmptyRect(Utils::Vector2{10, 10}, 20, 20, rgb::green);
        OGL::DrawEmptyRect(Utils::Vector2{(float)m_viewport[2] - 30, (float)m_viewport[3] - 30}, 20, 20, rgb::green);

        OGL::DrawEmptyRect(Utils::Vector2{5, 5}, (float)m_viewport[2] - 10, (float)m_viewport[3] - 10, rgb::red);

        // Draw a box at a fixed position in the world, for testing W2S.
        Utils::Vector3 test_point { 674.0f, -404.0f, 127.0f };
        Utils::Vector3 test_point_screen_coords{};
        if(W2S::WorldToScreenColumn(test_point, test_point_screen_coords, globals::MVP_Matrix, m_viewport[2], m_viewport[3])){
            Utils::Vector2 test_draw_pos{ test_point_screen_coords.x, test_point_screen_coords.y };
            OGL::DrawRect(test_draw_pos, 5, 5, rgb::green);
        }
    }

    int maxEnts = *(int*)(globals::hwdllBase + 0x843D58);
    for(int i = 0; i < maxEnts; i++)
    {
        globals::Entity* ent = (globals::Entity*)(*globals::entityList + 0x324 * i);
        globals::Entity* ply = *(globals::Entity**)(globals::playerBase);

        // Make sure the looped entity and player are valid pointers.
        if(ent == NULL || ply == NULL){
            return;
        }

        bool isEntPlayer = ply->type == 3;
        bool isEntNpc = ent->type == 4;

        if(ent->health > 0 && isEntNpc)
        {
            float distance = Utils::GetDistanceSqrt(ply->pos_feet, ent->pos_feet);

            float height = (ent->pos_head.z - ent->pos_feet.z);
            float width = height / 1.6f;

            height = (height*6.0f) / ((1.0f + distance) / 100.0f);
            width = (width*6.0f) / ((1.0f + distance) / 100.0f);

            Utils::Vector3 screen_coords{};
            if(W2S::WorldToScreenColumn(ent->pos_true, screen_coords, globals::MVP_Matrix, m_viewport[2], m_viewport[3])){
                Utils::Vector2 screen_coords_v2 = { screen_coords.x, screen_coords.y };
                screen_coords_v2.x -= width/2;
                screen_coords_v2.y -= height;

                HDC currentHDC = wglGetCurrentDC();
                if(!glFont.bBuilt || currentHDC != glFont.hdc){
                    glFont.Build(FONT_HEIGHT);
                }
                //float textPointX = glFont.CenterTextX(screen_coords_v2.x, screen_coords_v2.y, strlen(healthDisplay) * FONT_WIDTH);
                //float textPointY = screen_coords_v2.x - FONT_HEIGHT / 2;
                std::string nameDisplay("Name: Unknown Entity");
                std::string healthDisplay("Health: " + std::to_string((int)ent->health));
                glFont.Print(screen_coords_v2.x + 32, screen_coords_v2.y, rgb::red, "%s", nameDisplay.c_str());
                 glFont.Print(screen_coords_v2.x + 32, screen_coords_v2.y+12, rgb::red, "%s", healthDisplay.c_str());

	            glDisable(GL_TEXTURE_2D);
	            glColor3ub(rgb::red[0], rgb::red[1], rgb::red[2]);
	            glBegin(GL_LINE_LOOP);
		            glVertex2f(screen_coords_v2.x, screen_coords_v2.y);
		            glVertex2f(screen_coords_v2.x + width, screen_coords_v2.y);
		            glVertex2f(screen_coords_v2.x + width, screen_coords_v2.y + height);
		            glVertex2f(screen_coords_v2.x, screen_coords_v2.y + height);
	            glEnd();
	            glEnable(GL_TEXTURE_2D);
            }
        }
    }

    OGL::Reset();
}