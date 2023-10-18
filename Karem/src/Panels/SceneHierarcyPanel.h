#pragma once

#include "KaremEngine.h"

namespace Karem {

	class SceneHierarcyPanel
	{
	public:
		SceneHierarcyPanel() = default;
		SceneHierarcyPanel(const std::shared_ptr<Scene>& context)
			: m_ContextScene(context) {}

		void RenderImGUI();
	private:
		void MenuBar();
		void ViewportPanel();
		void EntityPropertiesPanel();
		void EntityListPanel();

	private:
		Entity m_SelectionContext;
		std::shared_ptr<Scene> m_ContextScene;
	};

}
