#include "Input.h"

namespace xwf {

	Input::InputMap Input::inputMap;

	Input::KeyState Input::GetKeyState(uint32_t keycode)
	{
		if ((keycode >=0) && (keycode < MAX_KEYS))
			return inputMap.map[keycode];
		return Input::KeyState {};
	}
	bool Input::isKeyPressed(uint32_t keycode)
	{
		return inputMap.map[keycode].isDown;
	}
	bool Input::isKeyReleased(uint32_t keycode)
	{
		return !inputMap.map[keycode].isDown;
	}
	bool Input::wasKeyHit(uint32_t keycode)
	{
		return ((!inputMap.map[keycode].isDown) && inputMap.map[keycode].wasDown);
	}
	void Input::processKeyboardInput(uint32_t keycode, bool wasDown, bool isDown)
	{
#ifdef _WIN32
		if (wasDown != isDown)
		{
			if (keycode >= 'A' && keycode <= 'Z')
			{
				uint32_t dewcinKeycode = keycode - 'A';
				inputMap.map[dewcinKeycode].isDown = isDown;
				inputMap.map[dewcinKeycode].wasDown = wasDown;
			}
			else if (keycode == VK_UP)
			{
				inputMap.map[XWF_UP].isDown = isDown;
				inputMap.map[XWF_UP].wasDown = wasDown;
			}
			else if (keycode == VK_DOWN)
			{
				inputMap.map[XWF_DOWN].isDown = isDown;
				inputMap.map[XWF_DOWN].wasDown = wasDown;
			}
			else if (keycode == VK_LEFT)
			{
				inputMap.map[XWF_LEFT].isDown = isDown;
				inputMap.map[XWF_LEFT].wasDown = wasDown;
			}
			else if (keycode == VK_RIGHT)
			{
				inputMap.map[XWF_RIGHT].isDown = isDown;
				inputMap.map[XWF_RIGHT].wasDown = wasDown;
			}
			else if (keycode >= '0' && keycode <= '9')
			{
				uint32_t dc_keycode = keycode - '0' + XWF_0;
				inputMap.map[dc_keycode].isDown = isDown;
				inputMap.map[dc_keycode].wasDown = wasDown;
			}
			else if (keycode == VK_OEM_MINUS)
			{
				inputMap.map[XWF_MINUS].isDown = isDown;
				inputMap.map[XWF_MINUS].wasDown = wasDown;
			}
			else if (keycode == VK_OEM_PLUS)
			{
				inputMap.map[XWF_PLUS].isDown = isDown;
				inputMap.map[XWF_PLUS].wasDown = wasDown;
			}
			else if (keycode == VK_SHIFT)
			{
				inputMap.map[XWF_SHIFT].isDown = isDown;
				inputMap.map[XWF_SHIFT].wasDown = wasDown;
			}
			else if (keycode == VK_CONTROL)
			{
				inputMap.map[XWF_CONTROL].isDown = isDown;
				inputMap.map[XWF_CONTROL].wasDown = wasDown;
			}
			else if (keycode == VK_MENU)
			{
				inputMap.map[XWF_ALT].isDown = isDown;
				inputMap.map[XWF_ALT].wasDown = wasDown;
			}
			else if (keycode == VK_SPACE)
			{
				inputMap.map[XWF_SPACE].isDown = isDown;
				inputMap.map[XWF_SPACE].wasDown = wasDown;
			}
			else if (keycode == VK_ESCAPE)
			{
				inputMap.map[XWF_ESCAPE].isDown = isDown;
				inputMap.map[XWF_ESCAPE].wasDown = wasDown;
			}
			else if (keycode == VK_CAPITAL)
			{
				inputMap.map[XWF_CAPSLOCK].isDown = isDown;
				inputMap.map[XWF_CAPSLOCK].wasDown = wasDown;
			}
			else if (keycode == VK_TAB)
			{
				inputMap.map[XWF_TAB].isDown = isDown;
				inputMap.map[XWF_TAB].wasDown = wasDown;
			}
			else if (keycode == VK_RETURN)
			{
				inputMap.map[XWF_ENTER].isDown = isDown;
				inputMap.map[XWF_ENTER].wasDown = wasDown;
			}
			else if (keycode == VK_BACK)
			{
				inputMap.map[XWF_BACKSPACE].isDown = isDown;
				inputMap.map[XWF_BACKSPACE].wasDown = wasDown;
			}
			else if (keycode == VK_OEM_3)
			{
				inputMap.map[XWF_TILDE].isDown = isDown;
				inputMap.map[XWF_TILDE].wasDown = wasDown;
			}
		}
#endif
	}
	
}