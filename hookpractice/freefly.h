#pragma once

#include "Utils.h"

namespace freefly {
	Utils::Vector3 GetStepVector(Utils::Vector3 player_angles, float amount);
	void PlayerStep(float step_amount, int dir=0);

	enum DIR {
		FORWARD,
		LEFT,
		BACK,
		RIGHT
	};
}