#pragma once
#include "ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer* window_container, HINSTANCE, std::string, std::string, int width, int height);
	bool ProcessMessages();
	HWND getHandle();
	~RenderWindow();
private:
	void RegisterWindowClass();
	WindowContainer* pWindowContainer;
	HWND handle = NULL;
	HINSTANCE hInstance = NULL;
	std::string window_title = "";
	std::wstring window_title_wide = L"";
	std::string window_class = "";
	std::wstring window_class_wide = L"";

	int width = 0;
	int height = 0;
};
