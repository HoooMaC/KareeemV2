#include "Core/Kareeem.h"
#include "CameraHandler.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "external/imgui/imgui_configuration.h"

namespace Karem {

	CameraHandler::CameraHandler()
	{
		m_AspectRatio = 16.0f / 9.0f;

		if (Karem::IsImGuiContextValid())
		{
			if (auto viewportWindow = ImGui::FindWindowByName("Viewport"))
				m_AspectRatio = viewportWindow->Size.x / viewportWindow->Size.y;
		}

		m_OrthographicCamera = OrthographicCamera(m_AspectRatio);
		m_PerspectiveCamera = PerspectiveCamera(m_AspectRatio, glm::radians(45.0f), 0.0001f, 1000.0f);
		m_CurrentCammeraType = Type::Orthographic;
	}

	CameraHandler::CameraHandler(const OrthographicCamera& camera)
		: m_OrthographicCamera(camera)
	{
		m_AspectRatio = 16.0f / 9.0f;

		if (Karem::IsImGuiContextValid())
		{
			if (auto viewportWindow = ImGui::FindWindowByName("Viewport"))
				m_AspectRatio = viewportWindow->Size.x / viewportWindow->Size.y;
		}

		m_PerspectiveCamera = PerspectiveCamera(m_AspectRatio, glm::radians(45.0f), 0.0001f, 1000.0f);
		m_CurrentCammeraType = Type::Orthographic;
	}

	CameraHandler::CameraHandler(const PerspectiveCamera& camera)
		: m_PerspectiveCamera(camera)
	{
		m_AspectRatio = 16.0f / 9.0f;

		if (Karem::IsImGuiContextValid())
		{
			if (auto viewportWindow = ImGui::FindWindowByName("Viewport"))
				m_AspectRatio = viewportWindow->Size.x / viewportWindow->Size.y;
		}

		m_OrthographicCamera = OrthographicCamera(m_AspectRatio);
		m_CurrentCammeraType = Type::Perspective;
	}

	CameraHandler::~CameraHandler()
	{
	}

}

