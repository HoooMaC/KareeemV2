#pragma once

#include "KaremEngine.h"

class Sandbox : public Karem::Application
{
public:
	Sandbox();
	~Sandbox();

	void Run() override;
	void EventHandler(Karem::Event& event) override;
protected:
	void Init() override;
	void Shutdown() override;

private:
	bool WindowCloseAction(Karem::WindowCloseEvent& event);
	
private:
	//Karem::Layers m_Layers;
	Karem::OrthographicCamera m_Camera;
	std::shared_ptr<Karem::Texture2D> m_Texture;

	float m_LastFrameTime = 0.0f;
};

