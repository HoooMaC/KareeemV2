#include "Panel.h"

#include "SceneHierarcyPanel.h"

#include "Scene/Components.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_internal.h>

namespace Karem {

	void Panels::SetNewSceneFuction(std::function<void()> function)
	{
		m_Menubar.newSceneFunction = function;
	}

	void Panels::SetSaveSceneAsFuction(std::function<void(std::string_view)> function)
	{
		m_Menubar.saveSceneAsFunction = function;
	}

	void Panels::SetOpenSceneFuction(std::function<void(std::string_view)> function)
	{
		m_Menubar.openSceneFunction = function;
	}

	void Panels::SetScene(const std::shared_ptr<Scene>& scene)
	{
		m_ContextScene = scene;
		m_SelectedEntity = {};
		m_MainCamera = {};
	}

	void Panels::RenderMenubar()
	{
		m_Menubar.Render();
	}

	void Panels::RenderHierarcyPanel()
	{
		if(m_Menubar.CheckPanelStatus(PanelsStatus::EntityList))
			m_HierarcyPanel.RenderEntityList(m_SelectedEntity, m_MainCamera, m_ContextScene, m_Menubar.GetPanelPointer(PanelsStatus::EntityList));

		if (m_Menubar.CheckPanelStatus(PanelsStatus::EntityComponent))
			m_HierarcyPanel.RenderEntityComponent(m_SelectedEntity, m_MainCamera, m_Menubar.GetPanelPointer(PanelsStatus::EntityComponent));

		if (m_Menubar.CheckPanelStatus(PanelsStatus::CameraPanel))
			m_HierarcyPanel.RenderCameraPanel(m_MainCamera, m_ContextScene, m_Menubar.GetPanelPointer(PanelsStatus::CameraPanel));
	}

	void Panels::EventHandler(Event& event)
	{
		m_Menubar.EventHandler(event);
	}

}