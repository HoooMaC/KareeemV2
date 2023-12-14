#pragma once

#include "Event/KeyCode.h"
#include "Event/MouseCode.h"

namespace Karem {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static float GetMousePositionX();
		static float GetMousePositionY();
	};

}
