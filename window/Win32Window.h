#pragma once

#include "helpers/WinInclude.h"
#include <string>

namespace xwf {

	class Win32Window {
	public:

		Win32Window(const Win32Window&) = delete;
		Win32Window operator=(const Win32Window&) = delete;
		static void Init(const wchar_t*);
		bool update();
		static HWND getHWND();

		inline static Win32Window& Get() 
		{
			static Win32Window instance;
			return instance;
		}
	private:
		HWND hwnd = nullptr;
		
		Win32Window() = default;
	};

}