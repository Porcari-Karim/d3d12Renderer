#include <iostream>
#include <algorithm>
#include <cassert>
#include <chrono>

#include <helpers/WinInclude.h>
#include <debug/DXDebugLayer.h>
#include <helpers/Helpers.h>
#include <window/Win32Window.h>
#include <window/Input.h>
#include <renderer/DX12Context.h>


int main()
{
	xwf::DXDebugLayer::Get().Init();

	{
		xwf::Win32Window::Init(L"Test WIndow");
		std::cout << xwf::Win32Window::getHWND() << std::endl;
		if (xwf::DX12Context::Get().Init())
		{
            

			while (xwf::Win32Window::Get().update()) {
				
				xwf::DX12Context::Get().TestRendering();

			}

			xwf::DX12Context::Get().Shutdown();
		}
	}
	xwf::DXDebugLayer::Get().Shutdown();
	std::cout << "Exited normally\n";
}