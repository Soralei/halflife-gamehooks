#pragma once
#pragma comment(lib, "OPENGL32.lib")

#include <Windows.h>
#include <stdio.h>
#include <gl/GL.h>

#include "Utils.h"

namespace rgb {
	const GLubyte red[3] = {255, 0, 0};
	const GLubyte green[3] = {0, 255, 0};
	const GLubyte blue[3] = {0, 0, 255};
}

namespace OGL {
	void SetOrtho();
	void Reset();
	void DrawRect(Utils::Vector2 vertices, float width, float height, const GLubyte color[3]);
	void DrawEmptyRect( Utils::Vector2 vertices, float width, float height, const GLubyte color[3]);
	void DrawLine( Utils::Vector2 from, Utils::Vector2 to, const GLubyte color[3]);

	class Font {
	public:
		bool bBuilt = false;
		unsigned int base;
		HDC hdc = nullptr;
		int height;
		int width;

		void Build(int height);
		void Print(float x, float y, const unsigned char color[3], const char* format, ...);
		Utils::Vector3 CenterTextXY(float x, float y, float width, float height, float textWidth, float textHeight);
		float CenterTextX(float x, float width, float textWidth);
	};
}