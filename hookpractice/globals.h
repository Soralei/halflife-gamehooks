#pragma once

#include "Utils.h"

#define STR_MERGE_IMPL(a, b) a##b // Concatenate the value of _COUNTER_ to _pad
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b) // Convert _COUNTER_ into a literal value.
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size] // Defines the char padding's unique name.
#define DEFINE_MEMBER_N(type, name, offset) struct { unsigned char MAKE_PAD(offset); type name; } // Defines a struct that can be accessed via a union inside of a class.

namespace globals {
	extern bool freeflyEnabled;
	extern bool freecamEnabled;
	extern bool espEnabled;
	extern bool espDebugEnabled;

	extern DWORD hwdllBase;
	extern DWORD* playerBase;
	extern DWORD* entityList;
	extern float* MVP_Matrix;

	class Player {
		public:
			union {
				DEFINE_MEMBER_N(Utils::Vector3, pos_true, 0x88);
				DEFINE_MEMBER_N(Utils::Vector3, pos_feet, 0x144);
				DEFINE_MEMBER_N(Utils::Vector3, pos_head, 0x150);
				DEFINE_MEMBER_N(INT, type, 0x188);
				DEFINE_MEMBER_N(FLOAT, health, 0x1E0);
			};
	};

	class Entity {
		public:
			union {
				DEFINE_MEMBER_N(Utils::Vector3, pos_true, 0x88);
				DEFINE_MEMBER_N(Utils::Vector3, pos_feet, 0x144);
				DEFINE_MEMBER_N(Utils::Vector3, pos_head, 0x150);
				DEFINE_MEMBER_N(INT, type, 0x188);
				DEFINE_MEMBER_N(FLOAT, health, 0x1E0);
			};
	};

	/*
	struct Entity {
		static const DWORD pos_x = 0x88;
		static const DWORD pos_y = 0x8C;
		static const DWORD pos_z = 0x90;
		static const DWORD pos_feet = 0x14C;
		static const DWORD pos_head = 0x158;
		static const DWORD health = 0x1E0;
		static const DWORD type = 0x188;
		static const DWORD struct_size = 0x324;
	};
	*/

	struct PlayerCamera {
		float* pos_x = (float*)0x048CAEE8;
		float* pos_y = (float*)0x048CAEEC;
		float* pos_z = (float*)0x048CAEF0;

		float* pitch = (float*)(hwdllBase + 0x1203AAC);
		float* yaw = (float*)(hwdllBase + 0x1203AAC + 0x4);
		float* roll = (float*)(hwdllBase + 0x1203AAC + 0x8);
	};
}