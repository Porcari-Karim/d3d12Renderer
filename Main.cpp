#include <iostream>
#include <algorithm>
#include <cassert>
#include <chrono>

#include <helpers/WinInclude.h>
#include <debug/DXDebugLayer.h>
#include <helpers/Helpers.h>
#include <window/Window.h>
#include <window/Input.h>
#include <renderer/DXContext.h>


int main()
{
	xwf::DXDebugLayer::Get().Init();

	if (xwf::DXContext::Get().Init())
	{
		xwf::DXContext::Get().GetDevice() = nullptr;
		xwf::Window::init();
		xwf::Window wnd(L"Test WIndow");
		while (wnd.update()) {
			
		}

		xwf::DXContext::Get().Shutdown();
	}
	xwf::DXDebugLayer::Get().Shutdown();
	std::cout << "Exited normally\n";
}