#pragma once

namespace Utils {
	struct Vector2 { float x, y; };
	struct Vector3 { float x, y, z; };
	struct Vector4 { float x, y, z, w; };

	float GetDistanceSqrt(Vector3 fromPos, Vector3 toPos);
	float GetDistance(Vector3 fromPos, Vector3 toPos);

	Vector3 AddVectors(Vector3 src, Vector3 dst);
	float DegreeToRadian(float degree);
	Vector3 VectorDegreeToRadian(Vector3 degrees);
}