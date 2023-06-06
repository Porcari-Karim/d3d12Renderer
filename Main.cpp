#include "WinInclude.h"
#include <iostream>

#include <algorithm>
#include <cassert>
#include <chrono>

#include "Helpers.h"
#include "Window.h"
#include "Input.h"

int main()
{
	xwf::Window::init();
	xwf::Window wnd(L"Test WIndow");
	while (wnd.update()) {
		if (xwf::Input::isKeyPressed(XWF_A))
			std::cout << "A\n";
	}
	std::cout << "Exited normally\n";
}