#include "Window.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <stdint.h>
#include "Input.h"

namespace xwf {

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        {
            uint32_t VkCode = wParam;
            bool wasDown = (lParam & (1 << 30)) != 0;
            bool isDown = (lParam & (1 << 31)) == 0;
            Input::processKeyboardInput(VkCode, wasDown, isDown);
        }
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }


    Window::Window(const wchar_t* title)
    {
        this->hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            L"XWindow Base Class",                     // Window class
            title,    // Window text
            WS_OVERLAPPEDWINDOW,            // Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

            NULL,       // Parent window    
            NULL,       // Menu
            GetModuleHandle(nullptr),  // Instance handle
            NULL        // Additional application data
        );

        if (this->hwnd == NULL)
        {
            throw std::exception("Failed to create HWND");
        }
        std::cout << "Created Window\n";
        ShowWindow(this->hwnd, SW_SHOWNORMAL);
    }

    Window::~Window()
    {

    }

    void Window::init()
    {
        const wchar_t wClassName[] = L"XWindow Base Class";
        WNDCLASS wc = {};

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = wClassName;

        RegisterClass(&wc);

    }

    bool Window::update()
    {
        MSG msg = { };
        PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
        if (msg.message == WM_QUIT)
        {
            std::cout << "Window destroyed\n";
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
          
        return true;
    }

}
