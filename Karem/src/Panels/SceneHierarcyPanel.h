#pragma once

#include "Scene/Scene.h"

#include <memory>

namespace Karem {

	class SceneHierarcyPanel
	{
		friend class MenubarPanel;

		enum class PanelsStatus : int16_t
		{
			EntityList = 0,
			EntityComponent,
			CameraPanel,
			Counts
		};
	public:
		SceneHierarcyPanel() = default;
		SceneHierarcyPanel(const std::shared_ptr<Scene>& context)
		{
			static_assert((uint64_t)PanelsStatus::Counts * sizeof(bool) == sizeof(m_Panels), "Size mismatch");
			memset(m_Panels, 0, sizeof(m_Panels));
		}

		void Render(Entity& selectedEntity, Entity& mainCamera, const std::shared_ptr<Scene>& scene);


		bool* GetPanelStatus() { return m_Panels; }
	private:
		//void DrawEntityTree(Entity entity, TagComponent& tag);
		void DrawEntityComponents(Entity entity, Entity& selectedEntity, Entity& mainCamera);

	private:
		bool m_Panels[(int16_t)PanelsStatus::Counts];
	};

}
