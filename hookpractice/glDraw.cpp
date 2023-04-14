#include "pch.h"

#include "glDraw.h"
#include "globals.h"

#include <iostream>

DWORD hwdllBase = (DWORD)GetModuleHandle(L"hw.dll");
DWORD* fullscreenPtr = (DWORD*)(hwdllBase + 0xAB7700);
bool* isFullscreen = (bool*)fullscreenPtr;

DWORD* totalGameWidth = (DWORD*)(hwdllBase + 0x6C3ABC); // Width of the game.
DWORD* totalGameHeight = (DWORD*)(hwdllBase + 0x6C3AC0); // Height of the game.

DWORD* resolutionWidth = (DWORD*)(hwdllBase + 0x166FBC); // This is what fullscreen normal uses

int viewport_offset = 0;
int viewport_width = 0;
int viewport_height = 0;

// Set mode to orthographic.
void OGL::SetOrtho(){
	int monitor_width = GetSystemMetrics(SM_CXSCREEN);
	int monitor_height = GetSystemMetrics(SM_CYSCREEN);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	//if(*isFullscreen){}

	if(globals::espEnabled && globals::espDebugEnabled){
		if(GetAsyncKeyState(VK_NUMPAD7) & 0x8000){
			viewport_offset -= 10;
			Sleep(25);
		}

		if(GetAsyncKeyState(VK_NUMPAD8) & 0x8000){
			viewport_offset += 10;
			Sleep(25);
		}

		if(GetAsyncKeyState(VK_NUMPAD9) & 0x8000){
			viewport_offset = 0;
			Sleep(25);
		}

		if(GetAsyncKeyState(VK_NUMPAD4) & 0x8000){
			viewport_width -= 10;
			Sleep(25);
		}

		if(GetAsyncKeyState(VK_NUMPAD5) & 0x8000){
			viewport_width += 10;
			Sleep(25);
		}

		if(GetAsyncKeyState(VK_NUMPAD6) & 0x8000){
			viewport_width = 0;
			Sleep(25);
		}

		if(GetAsyncKeyState(VK_NUMPAD1) & 0x8000){
			viewport_height -= 10;
			Sleep(25);
		}

		if(GetAsyncKeyState(VK_NUMPAD2) & 0x8000){
			viewport_height += 10;
			Sleep(25);
		}

		if(GetAsyncKeyState(VK_NUMPAD3) & 0x8000){
			viewport_height = 0;
			Sleep(25);
		}
	}

	viewport[2] = *(int*)totalGameWidth;
	viewport[3] = *(int*)totalGameHeight;

	viewport[2] = viewport[2] + viewport_width;
	viewport[3] = viewport[3] + viewport_height;

	glViewport(viewport_offset, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

// Reset the view.
void OGL::Reset(){
	glPopMatrix();
	glPopAttrib();
}

// Immediate mode drawing.
void OGL::DrawRect(Utils::Vector2 vertices, float width, float height, const GLubyte color[3]){
	glDisable(GL_TEXTURE_2D); // THIS IS KEY. Otherwise it will only draw when the console is open.
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex2f(vertices.x, vertices.y);
	glVertex2f(vertices.x + width, vertices.y);
	glVertex2f(vertices.x + width, vertices.y + height);
	glVertex2f(vertices.x, vertices.y + height);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void OGL::DrawEmptyRect(Utils::Vector2 pos, float width, float height, const GLubyte color[3]){
	glDisable(GL_TEXTURE_2D);
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_LINE_LOOP);
		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + width, pos.y);
		glVertex2f(pos.x + width, pos.y + height);
		glVertex2f(pos.x, pos.y + height);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void OGL::DrawLine(Utils::Vector2 from, Utils::Vector2 to, const GLubyte color[3]){
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
		glVertex2f(from.x, from.y);
		glVertex2f(to.x, to.y);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void OGL::Font::Build(int height){
	hdc = wglGetCurrentDC();
	base = glGenLists(96);
	HFONT hFont = CreateFontA(-height, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Consolas");
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
	wglUseFontBitmaps(hdc, 32, 96, base);
	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);

	bBuilt = true;
}

void OGL::Font::Print(float x, float y, const unsigned char color[3], const char* format, ...){
	glDisable(GL_TEXTURE_2D);
	glColor3ub(color[0], color[1], color[2]);
	glRasterPos2f(x, y);

	char text[128];
	va_list args;

	va_start(args, format);
	vsprintf_s(text, 128, format, args);
	va_end(args);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
	glEnable(GL_TEXTURE_2D);
}

Utils::Vector3 OGL::Font::CenterTextXY(float x, float y, float width, float height, float textWidth, float textHeight){
	Utils::Vector3 text;
	text.x = x + (width - textWidth) / 2;
	text.y = y + textHeight;
	return text;
}

float OGL::Font::CenterTextX(float x, float width, float textWidth){
	if(width > textWidth){
		float delta = width - textWidth;
		return (x + (delta / 2));
	} else {
		float delta = textWidth - width;
		return (x - (delta / 2));
	}
}