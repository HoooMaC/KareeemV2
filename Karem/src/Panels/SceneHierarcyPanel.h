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
		void DrawEntityTree(Entity entity, TagComponent& tag);
		void DrawEntityComponents(Entity entity);
		
	private:
		Entity m_SelectedEntity;
		std::shared_ptr<Scene> m_ContextScene;
	};

}
