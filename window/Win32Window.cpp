#include "Win32Window.h"
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
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }


    void Win32Window::Init(const wchar_t* title)
    {
        const wchar_t wClassName[] = L"XWin32Window Base Class";
        WNDCLASSEXW wc = {};

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = wClassName;
        wc.style = CS_OWNDC;
        wc.hCursor = LoadCursorW(nullptr, IDC_HAND);
        wc.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
        wc.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);
        wc.cbSize = sizeof(wc); //Important !!!! The most important !!!


        
        RegisterClassExW(&wc);

        Get().hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            L"XWin32Window Base Class",                     // Win32Window class
            title,    // Win32Window text
            WS_OVERLAPPEDWINDOW,            // Win32Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

            NULL,       // Parent window    
            NULL,       // Menu
            GetModuleHandle(nullptr),  // Instance handle
            NULL        // Additional application data
        );

        if (Get().hwnd == NULL)
        {
            throw std::exception("Failed to create HWND");
        }
        std::cout << "Created Win32Window\n";
        ShowWindow(Get().hwnd, SW_SHOWNORMAL);

    }

    bool Win32Window::update()
    {
        MSG msg = { };
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                std::cout << "Win32Window destroyed\n";
                return false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
          
        return true;
    }

    HWND Win32Window::getHWND()
    {
        return hwnd;
    }

}
