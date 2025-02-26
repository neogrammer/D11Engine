#pragma once
#include "WindowContainer.h"
class Engine : public WindowContainer
{
	RenderWindow renderWindow;
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();

};