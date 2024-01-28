#pragma once

#include "Scene/Scene.h"

#include "PanelsStatus.h"

#include <memory>

namespace Karem {

	class SceneHierarcyPanel
	{
		friend class MenubarPanel;
	public:
		SceneHierarcyPanel() = default;

		void RenderEntityList(Entity& selectedEntity, Entity& mainCamera, const std::shared_ptr<Scene>& scene, bool* panelPointer);
		void RenderEntityComponent(Entity& selectedEntity, Entity& mainCamera, bool*  panelPointer);
		void RenderCameraPanel(Entity& mainCamera, const std::shared_ptr<Scene>& scene, bool*  panelPointer);
	private:

		//void DrawEntityTree(Entity entity, TagComponent& tag);
		void DrawEntityComponents(Entity& selectedEntity, Entity& mainCamera);
	};

}
