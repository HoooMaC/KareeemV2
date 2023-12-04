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
		
		void SetContextScene(std::shared_ptr<Scene>& context);
	private:
		void DrawEntityTree(Entity entity, TagComponent& tag);
		void DrawEntityComponents(Entity entity);
		
	private:
		Entity m_SelectedEntity;
		Entity m_MainCamera;
			
		std::shared_ptr<Scene> m_ContextScene;
	};

}
