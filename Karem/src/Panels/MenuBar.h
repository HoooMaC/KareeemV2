#pragma once

#include <vector>

namespace Karem {

	class SceneHierarcyPanel;

	class MenuBar
	{
		friend SceneHierarcyPanel;
	public:
		static void Render();

	private:
		static bool showEntityList;
		static bool showEntityComponent;
		static bool showCameraPanel;
	};

}
