#include "WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer* window_container, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    pWindowContainer = window_container;
    this->hInstance = hInstance;
    this->width = width;
    this->height = height;
    this->window_title = window_title;
    this->window_title_wide = StringConverter::StringToWide(window_title);
    this->window_class = window_class;
    this->window_class_wide = StringConverter::StringToWide(window_class);

    this->RegisterWindowClass();

    this->handle = CreateWindowEx(0, this->window_class_wide.c_str(), this->window_class_wide.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        0, 0, this->width, this->height, NULL, NULL, this->hInstance, pWindowContainer);
    if (this->handle == NULL)
    {
        ErrorLogger::Log(GetLastError(), "CreateWindowEx failed for Window: " + this->window_title);
        return false;
    }

    ShowWindow(this->handle, SW_SHOW);
    SetForegroundWindow(this->handle);
    SetFocus(this->handle);

    //window_container->SetUpRaw(this->handle);

    return true;
}

bool RenderWindow::ProcessMessages()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_NULL)
    {
        if (!IsWindow(this->handle))
        {
            this->handle = NULL;
            UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
            return false;
        }
    }

    return true;
}

HWND RenderWindow::getHandle()
{
    return this->handle;
}

RenderWindow::~RenderWindow()
{
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        DestroyWindow(hwnd);
        return 0;
    }
    break;
    default:
    {
        WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
}


LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
    case WM_NCCREATE:
        {
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
            if (pWindow == nullptr)
            {
                ErrorLogger::Log("Critical Error: Pointer to window container is null during WM_NCCREATE");
                exit(-1);
            }
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
            return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
        }
        break;
    break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

}


void RenderWindow::RegisterWindowClass()
{
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = HandleMessageSetup;
    wc.hInstance = this->hInstance;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = this->window_class_wide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszMenuName = NULL;
    RegisterClassEx(&wc);
}
