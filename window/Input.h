#pragma once
#include <helpers/WinInclude.h>
#include <stdint.h>

#include "WinKeyCodes.h"

#define MAX_KEYS 52

namespace xwf {

	class Input {
	public:
		struct KeyState
		{
			bool wasDown;
			bool isDown;
		};

		struct InputMap
		{
			KeyState map[MAX_KEYS];
		};

		static KeyState GetKeyState(uint32_t keycode);
		static bool isKeyPressed(uint32_t keycode);
		static bool isKeyReleased(uint32_t keycode);
		static bool wasKeyHit(uint32_t keycode);

	private:
		static InputMap inputMap;

		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		static void processKeyboardInput(uint32_t keycode, bool wasDown, bool isDown);

	};

}