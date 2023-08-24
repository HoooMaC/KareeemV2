#pragma once
#include <KaremEngine.h>

class ControlLayer : public Karem::Layer
{
public:
	ControlLayer(const std::string& layerName = "control");

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void OnUpdate();

	virtual void EventHandler(Karem::Event& event);

	inline void AddData(std::pair<std::string*, bool*> data) { m_LayerStatus.push_back(data); }
private:
	std::vector<std::pair<std::string*, bool*>> m_LayerStatus;
};
