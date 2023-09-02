#pragma once

#include "KaremEngine.h"

class ControlLayer : Karem::Layer
{
public:
	ControlLayer(Karem::Layers& container, const std::string& layerName = "control")
		: Layer(layerName), m_LayerContainerRef(container) {}

	void OnUpdate(Karem::TimeStep ts) override;
	void UpdateLayerCounter(uint32_t count) { m_LayerCount = count; }

private:
	Karem::Layers& m_LayerContainerRef;
	uint32_t m_LayerCount = 0;
};
