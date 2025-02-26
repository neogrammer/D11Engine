#include "Engine.h"
#include "WindowContainer.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	//keyboard.EnableAutoRepeatChars();
	if (!renderWindow.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}
	if (!graphics.Initialize(renderWindow.getHandle(), width, height))
	{
		return false;
	}

	return true;
}

bool Engine::ProcessMessages()
{
	return this->renderWindow.ProcessMessages();
}

void Engine::Update()
{
	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
		std::string outmsg = "Char: ";
		outmsg += ch;
		outmsg += "\n";
		std::wstring ws(outmsg.begin(), outmsg.end());
		OutputDebugStringW(ws.c_str());
	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent e = keyboard.ReadKey();
		unsigned char ch = e.GetKeyCode();
		std::string outmsg = "Key: ";
		if (e.IsPress())
		{
			outmsg += "Pressed: ";
		}
		else
		{
			outmsg += "Released: ";
		}
		outmsg += ch;
		outmsg += "\n";
		std::wstring ws(outmsg.begin(), outmsg.end());
		OutputDebugStringW(ws.c_str());
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			std::string outmsg = "X: ";
			outmsg += std::to_string(me.GetPosX());
			outmsg += ", Y: ";
			outmsg += std::to_string(me.GetPosY());
			outmsg += "\n";
			OutputDebugStringA(outmsg.c_str());
		}
	}
}

void Engine::RenderFrame()
{
	graphics.RenderFrame();
}
