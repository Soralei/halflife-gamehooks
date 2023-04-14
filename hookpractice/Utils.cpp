
#include "pch.h"

#include "Utils.h"
#include <math.h>

constexpr auto PI = 3.1415927f;

float Utils::GetDistanceSqrt(Vector3 fromPos, Vector3 toPos){
	float dx = toPos.x - fromPos.x;
	float dy = toPos.y - fromPos.y;
	float dz = toPos.z - fromPos.z;

	return sqrt(dx*dx + dy*dy + dz*dz);
}

float Utils::GetDistance(Vector3 fromPos, Vector3 toPos){
	float dx = toPos.x - fromPos.x;
	float dy = toPos.y - fromPos.y;
	float dz = toPos.z - fromPos.z;

	return dx*dx + dy*dy + dz*dz;
}

Utils::Vector3 Utils::AddVectors(Vector3 src, Vector3 dst){
    Vector3 sum;
    sum.x = src.x + dst.x;
    sum.y = src.y + dst.y;
    sum.z = src.z + dst.z;
    return sum;
}

float Utils::DegreeToRadian(float degree){
    return degree * (PI / 180);
}

Utils::Vector3 Utils::VectorDegreeToRadian(Vector3 degrees)
{
    Vector3 radians;
    radians.x = degrees.x * (PI / 180);
    radians.y = degrees.y * (PI / 180);
    radians.z = degrees.z * (PI / 180);
    return radians;
}