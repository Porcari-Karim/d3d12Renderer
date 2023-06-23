#include <iostream>
#include <algorithm>
#include <cassert>
#include <chrono>

#include <helpers/WinInclude.h>
#include <debug/DXDebugLayer.h>
#include <helpers/Helpers.h>
#include <window/Win32Window.h>
#include <window/Input.h>
#include <renderer/DX12Renderer.h>
#include <renderer/DX12Context.h>


int main()
{
	xwf::DXDebugLayer::Get().Init();

	{
		xwf::Win32Window::Init(L"Test WIndow");
		std::cout << xwf::Win32Window::getHWND() << std::endl;

        

		if (xwf::DX12Context::Get().Init())
		{
            std::vector<xwf::Vertex> vertexBufferData = {
                {{0.0, 0.5, 1.0}, {1.0, 0.0, 0.0}},
                {{0.5, -0.5, 1.0}, {0.0, 1.0, 0.0}},
                {{-0.5, -0.5, 1.0}, {0.0, 0.0, 1.0}},

            };

            std::vector<xwf::Vertex> vertexBufferData2 = {
                { {0.0, -0.5, 1.0}, {0.0, 0.0, 1.0}},
                { {-0.5, 0.5, 1.0}, {0.0, 0.0, 1.0}},
                { {0.5, 0.5, 1.0}, {0.0, 0.0, 1.0}}
            };

            std::vector<uint32_t> indexBufferData = { 0u, 1u, 2u};


            xwf::DX12Renderer renderer = {};
            renderer.LoadScene();
            renderer.CreateMeshFromData(vertexBufferData, indexBufferData);
            renderer.CreateMeshFromData(vertexBufferData2, indexBufferData);
			while (xwf::Win32Window::Get().update()) {
                renderer.RenderScene();
			}

			xwf::DX12Context::Get().Shutdown();
		}
	}
	xwf::DXDebugLayer::Get().Shutdown();
	std::cout << "Exited normally\n";
}