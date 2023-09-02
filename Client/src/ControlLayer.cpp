#include "ControlLayer.h"

#include "imgui.h"
#include "imgui_setup.h"

void ControlLayer::OnUpdate(Karem::TimeStep ts)
{
	ImGui::Begin("Layer Control");

	for (int i = 0; i < m_LayerCount; i++)
	{
		std::shared_ptr<Karem::Layer>& layer = m_LayerContainerRef.GetLayerAt(i);
		std::pair<std::string&, bool&> layerData = layer->GetLayerData();
		ImGui::Checkbox(layerData.first.c_str(), &layerData.second);

		if (i > 0)
		{
			ImGui::SameLine();
			std::string buttonName = "Make up " + layerData.first;
			if (ImGui::Button(buttonName.c_str()))
			{
				std::swap(m_LayerContainerRef.GetLayerAt(i), m_LayerContainerRef.GetLayerAt(i - 1));
			}
		}

		if (i < m_LayerCount - 1)
		{
			ImGui::SameLine();
			std::string buttonName = "Make down " + layerData.first;
			if (ImGui::Button(buttonName.c_str()))
			{
				std::swap(m_LayerContainerRef.GetLayerAt(i), m_LayerContainerRef.GetLayerAt(i + 1));
			}
		}
	}

	// untuk ImGUI, ada loopingnya tersendiri
	// berarti ada list atau stacknya sendiri
	ImGui::End();
}
