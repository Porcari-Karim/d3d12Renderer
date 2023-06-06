#pragma once

#include "WinInclude.h"
#include <string>

namespace xwf {

	class Window {
	public:

		Window(const wchar_t*);
		~Window();
		static void init();
		bool update();
		
	private:
		HWND hwnd = nullptr;
	};

}