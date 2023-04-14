#pragma once

#include "Utils.h"

// Source: https://guidedhacking.com/threads/world2screen-direct3d-and-opengl-worldtoscreen-functions.8044/
namespace W2S {
    bool WorldToScreenRow(Utils::Vector3 pos, Utils::Vector3 &screen, float matrix[16], int windowWidth, int windowHeight);
    bool WorldToScreenColumn(Utils::Vector3 pos, Utils::Vector3 &screen, float matrix[16], int windowWidth, int windowHeight);
}