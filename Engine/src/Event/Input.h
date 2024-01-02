#pragma once

#include "Event/KeyCode.h"
#include "Event/MouseCode.h"

namespace Karem {

	class Input
	{
	public:
		static bool IsKeyPressed(Key keycode);
		static bool IsKeyPressed(int16_t keycode);

		static bool IsMouseButtonPressed(Mouse button);
		static bool IsMouseButtonPressed(int16_t button);
		static float GetMousePositionX();
		static float GetMousePositionY();
	};

}
