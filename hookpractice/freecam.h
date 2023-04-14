#pragma once

#include "Utils.h"

namespace freecam {
	bool EnableFreeCam();
	bool DisableFreeCam();
	void FreecamStep(float stepAmount, int dir=0);
	Utils::Vector3 GetStepVector(Utils::Vector3 player_angles, float amount);

	enum DIR
	{
		FORWARD,
		LEFT,
		BACK,
		RIGHT,
	};
}