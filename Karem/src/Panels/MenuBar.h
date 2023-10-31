#pragma once

#include <vector>

namespace Karem {

	enum class Panels
	{
		EntityList,
		EntityComponent
	};

	class SceneHierarcyPanel;

	class MenuBar
	{
		friend SceneHierarcyPanel;
	public:
		static void Render();

	public:
		static std::vector<bool> panelStatus;
	private:
		static bool showEntityList;
		static bool showEntityComponent;
	};

}
