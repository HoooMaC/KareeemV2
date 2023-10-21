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
		void ViewportPanel();
		void EntityPropertiesPanel();
		void EntityListPanel();

	private:
		Entity m_SelectedEntity;
		std::shared_ptr<Scene> m_ContextScene;
	};

}
