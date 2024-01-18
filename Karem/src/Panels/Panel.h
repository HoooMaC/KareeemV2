#pragma once

#include "KaremEngine.h"

#include "SceneHierarcyPanel.h"
#include "Menubar.h"

namespace Karem {

	class Panels
	{
	public:
		void SetScene(const std::shared_ptr<Scene>& scene);

		void RenderMenubar();
		void RenderHierarcyPanel();

		void EventHandler(Event& event);

		Entity& GetSelectedEntity() { return m_SelectedEntity; }

		void SetNewSceneFuction(std::function<void()> function);
		void SetSaveSceneAsFuction(std::function<void(std::string_view)> function);
		void SetOpenSceneFuction(std::function<void(std::string_view)> function);

	private:
		SceneHierarcyPanel m_HierarcyPanel;
		MenubarPanel m_Menubar;

		std::shared_ptr<Scene> m_ContextScene;
		Entity m_SelectedEntity;
		Entity m_MainCamera;
	};

}
