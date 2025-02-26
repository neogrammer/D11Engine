#pragma once
#include "RenderWindow.h"
#include <Keyboard.h>
#include <Mouse.h>
#include "Graphics/Graphics.h"


class WindowContainer
{
public:
	WindowContainer();

//	bool SetUpRaw(HWND hwnd);
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	RenderWindow render_window;
	Mouse mouse;
	Keyboard keyboard;
	Graphics graphics;
};