#include "ControlLayer.h"

#include <imgui.h>

ControlLayer::ControlLayer(const std::string& layerName)
	: Layer(layerName)
{
}

void ControlLayer::OnAttach()
{
}

void ControlLayer::OnDetach()
{
}

void ControlLayer::OnUpdate()
{
	ImGui::Begin("Layer Control");

	for (auto [layerName, status] : m_LayerStatus)
	{
		ImGui::Checkbox(layerName->c_str(), status);
	}

	ImGui::End();
}

void ControlLayer::EventHandler(Karem::Event& event)
{
}
