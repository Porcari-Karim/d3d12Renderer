#include "helpers/WinInclude.h"
#include <debug/DebugLayer.h>
#include <iostream>

#include <algorithm>
#include <cassert>
#include <chrono>

#include <helpers/Helpers.h>
#include <window/Window.h>
#include <window/Input.h>

using namespace Microsoft::WRL;

int main()
{
	xwf::DebugLayer::Get().Init();
	xwf::Window::init();
	xwf::Window wnd(L"Test WIndow");

	{
	ComPtr<ID3D12Device9> device;
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

	while (wnd.update()) {
		if (xwf::Input::isKeyPressed(XWF_A))
			std::cout << "A\n";
	}
	}
	xwf::DebugLayer::Get().Shutdown();
	std::cout << "Exited normally\n";
}