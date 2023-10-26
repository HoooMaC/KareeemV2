#include "Core/Kareeem.h"
#include "CameraHandler.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "external/imgui/imgui_configuration.h"

namespace Karem {

	CameraHandler::CameraHandler()
	{
		// TODO : NEED TO BE SURE
		if (Karem::IsImGuiContextValid())
		{
			auto viewportWindow = ImGui::FindWindowByName("Viewport");
			if (viewportWindow)
			{
				auto viewportWindowSize = viewportWindow->Size;
				m_AspectRatio = viewportWindowSize.x / viewportWindowSize.y;
			}
			else
			{
				m_AspectRatio = 16.0f / 9.0f;
			}
		}
		else
		{
			m_AspectRatio = 16.0f / 9.0f;
		}
		m_OrthographicCamera = OrthographicCamera(m_AspectRatio);
		m_PerspectiveCamera = PerspectiveCamera(m_AspectRatio, glm::radians(45.0f), 0.0001f, 1000.0f);
		m_CurrentCammeraType = Type::Orthographic;
	}

	CameraHandler::CameraHandler(const OrthographicCamera& camera)
		: m_OrthographicCamera(camera)
	{
		// TODO : NEED TO BE SURE
		if (Karem::IsImGuiContextValid())
		{
			auto viewportWindow = ImGui::FindWindowByName("Viewport");
			if(viewportWindow)
			{
				auto viewportWindowSize = viewportWindow->Size;
				m_AspectRatio = viewportWindowSize.x / viewportWindowSize.y;
			}
			else
			{
				m_AspectRatio = 16.0f / 9.0f;
			}
		}
		else
		{
			m_AspectRatio = 16.0f / 9.0f;
		}

		m_PerspectiveCamera = PerspectiveCamera(m_AspectRatio, glm::radians(45.0f), 0.0001f, 1000.0f);
		m_CurrentCammeraType = Type::Orthographic;
	}

	CameraHandler::CameraHandler(const PerspectiveCamera& camera)
		: m_PerspectiveCamera(camera)
	{
		// TODO : NEED TO BE SURE
		if (Karem::IsImGuiContextValid())
		{
			auto viewportWindow = ImGui::FindWindowByName("Viewport");
			if (viewportWindow)
			{
				auto viewportWindowSize = viewportWindow->Size;
				m_AspectRatio = viewportWindowSize.x / viewportWindowSize.y;
			}
			else
			{
				m_AspectRatio = 16.0f / 9.0f;
			}
		}
		else
		{
			m_AspectRatio = 16.0f / 9.0f;
		}
		m_OrthographicCamera = OrthographicCamera(m_AspectRatio);
		m_CurrentCammeraType = Type::Perspective;
	}

	CameraHandler::~CameraHandler()
	{
	}

}

