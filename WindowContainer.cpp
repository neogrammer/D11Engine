#include "WindowContainer.h"
#include <memory>

WindowContainer::WindowContainer()
{
	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01; //Mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			ErrorLogger::Log(GetLastError(), "Failed to register raw input devices.");
			exit(-1);
		}

		raw_input_initialized = true;
	}
}

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//Keyboard Messages
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (keyboard.IsKeyAutoRepeat())
		{
			keyboard.OnKeyPressed(keycode);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				keyboard.OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		keyboard.OnKeyReleased(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (keyboard.IsCharAutoRepeat())
		{
			keyboard.OnChar(ch);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				keyboard.OnChar(ch);
			}
		}
		return 0;
	}
	//Mouse Messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouse.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			mouse.OnWheelDown(x, y);
		}
		return 0;
	}
	case WM_INPUT:
	{
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first

		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam); //Need to call DefWindowProc for WM_INPUT messages
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
//
//
//#include "WindowContainer.h"
//#include <memory>
//#include <strsafe.h>
//
//WindowContainer::WindowContainer()
//{
//	
//
//}
//
//bool WindowContainer::SetUpRaw(HWND hwnd)
//{
//	static bool raw_input_initialized = false;
//	if (raw_input_initialized = false)
//	{
//
//		Rid[0].usUsagePage = 0x01;
//		Rid[0].usUsage = 0x02;
//		Rid[0].dwFlags = 0;
//		Rid[0].hwndTarget = hwnd;
//
//		if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE)
//		{
//			ErrorLogger::Log(GetLastError(), "Failed to register raw input devices");
//			exit(-1);
//		}
//		raw_input_initialized = true;
//	}
//
//	return true;
//}
//
//LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch (uMsg)
//	{
//	case WM_INPUT:
//	{
//		UINT dwSize;
//
//		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
//		LPBYTE lpb = new BYTE[dwSize];
//
//		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
//			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
//
//		RAWINPUT* raw = (RAWINPUT*)lpb;
//		HRESULT hResult;
//		STRSAFE_LPWSTR szTempOutput = {};
//		if (raw->header.dwType == RIM_TYPEMOUSE)
//		{
//
//			mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
//
//			//hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH,
//			//	TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
//			//	raw->data.mouse.usFlags,
//			//	raw->data.mouse.ulButtons,
//			//	raw->data.mouse.usButtonFlags,
//			//	raw->data.mouse.usButtonData,
//			//	raw->data.mouse.ulRawButtons,
//			//	raw->data.mouse.lLastX,
//			//	raw->data.mouse.lLastY,
//			//	raw->data.mouse.ulExtraInformation);
//
//			//if (FAILED(hResult))
//			//{
//			//	// TODO: write error handler
//			//}
//
//
//
//			//OutputDebugString(szTempOutput);
//		}
//
//		delete[] lpb;
//		return 0;
//	}
//	break;
//
//		//Keyboard Messages
//	case WM_KEYUP:
//	{
//		unsigned char ch = static_cast<unsigned char>(wParam);
//		keyboard.OnKeyReleased(ch);
//		return 0;
//	}
//	break;
//	case WM_KEYDOWN:
//	{
//		unsigned char ch = static_cast<unsigned char>(wParam);
//		if (keyboard.IsKeyAutoRepeat())
//		{
//			keyboard.OnKeyPressed(ch);
//		}
//		else
//		{
//			const bool wasPressed = lParam & 0x40000000;
//			if (!wasPressed)
//			{
//				keyboard.OnKeyPressed(ch);
//			}
//		}
//		return 0;
//	}
//	break;
//	case WM_CHAR:
//	{
//		unsigned char ch = static_cast<unsigned char>(wParam);
//
//		if (keyboard.IsCharAutoRepeat())
//		{
//			keyboard.OnChar(ch);
//		}
//		else
//		{
//			const bool wasPressed = lParam & 0x40000000;
//			if (!wasPressed)
//			{
//				keyboard.OnChar(ch);
//			}
//		}
//		return 0;
//	}
//	break;
//	// mouse messages
//	case WM_MOUSEMOVE:
//	{
//		int x = LOWORD(lParam);
//		int y = HIWORD(lParam);
//	 	mouse.OnMouseMove(x, y);
//		return 0;
//	}
//	break;
//	case WM_LBUTTONDOWN:
//	{
//		int x = LOWORD(lParam);
//		int y = HIWORD(lParam);
//		mouse.OnLeftPressed(x, y);
//		return 0;
//
//	}
//	break;
//	case WM_RBUTTONDOWN:
//	{
//		int x = LOWORD(lParam);
//		int y = HIWORD(lParam);
//		mouse.OnRightPressed(x, y);
//		return 0;
//
//	}
//	break;
//	case WM_LBUTTONUP:
//	{
//		int x = LOWORD(lParam);
//		int y = HIWORD(lParam);
//		mouse.OnLeftReleased(x, y);
//		return 0;
//
//	}
//	break;
//	case WM_RBUTTONUP:
//	{
//		int x = LOWORD(lParam);
//		int y = HIWORD(lParam);
//		mouse.OnRightReleased(x, y);
//		return 0;
//
//	}
//	break;
//	case WM_MBUTTONDOWN:
//	{
//		int x = LOWORD(lParam);
//		int y = HIWORD(lParam);
//		mouse.OnMiddlePressed(x, y);
//		return 0;
//
//	}
//	break;
//	case WM_MBUTTONUP:
//	{
//		int x = LOWORD(lParam);
//		int y = HIWORD(lParam);
//		mouse.OnMiddleReleased(x, y);
//		return 0;
//
//	}
//	case WM_MOUSEWHEEL:
//	{
//		int x = LOWORD(lParam);
//		int y = HIWORD(lParam);
//		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
//		{
//			mouse.OnWheelUp(x, y);
//		}
//		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
//		{
//			mouse.OnWheelDown(x, y);
//		}
//	}
//	break;
//	default:
//		return DefWindowProc(hwnd, uMsg, wParam, lParam);
//		break;
//	}
//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}
