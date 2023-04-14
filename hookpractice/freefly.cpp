
#include "pch.h"

#include "freefly.h"
#include "globals.h"
#include "math.h"

Utils::Vector3 freefly::GetStepVector(Utils::Vector3 player_angles, float amount){
    Utils::Vector3 step{};

    float pitch = player_angles.x;
    float yaw = player_angles.y;

    step.x = cosf(Utils::DegreeToRadian(yaw)) * amount * cosf(Utils::DegreeToRadian(pitch));
    step.y = sinf(Utils::DegreeToRadian(yaw)) * amount * cosf(Utils::DegreeToRadian(pitch));
    step.z = sinf(Utils::DegreeToRadian(pitch)) * amount;
 
    return step;
}

void freefly::PlayerStep(float step_amount, int dir){
    globals::PlayerCamera cam{};

    Utils::Vector3 player_angles{};
    if(dir == freefly::DIR::LEFT){
        player_angles = {0, *cam.yaw + 90.0f, *cam.roll};
    } else if(dir == freefly::DIR::RIGHT){
        player_angles = {0, *cam.yaw + 270.0f, *cam.roll};
    } else {
        player_angles = {*cam.pitch, *cam.yaw, *cam.roll};
    }

    Utils::Vector3 step = freefly::GetStepVector(player_angles, step_amount);

    globals::Player* ply = *(globals::Player**)(globals::playerBase);
    if(dir == freefly::DIR::BACK){
        ply->pos_true.x -= step.x;
        ply->pos_true.y -= step.y;
        ply->pos_true.z -= step.z;
    } else {
        ply->pos_true.x += step.x;
        ply->pos_true.y += step.y;
        ply->pos_true.z += step.z;
    }
}