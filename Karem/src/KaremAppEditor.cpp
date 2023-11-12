#include "KaremAppEditor.h"

namespace Karem {

	// TEMPORARY -- this is should in Application.cpp
	void StartUpApplication(AppType type)
	{
		switch (type)
		{
			case Karem::AppType::None:
				break;
			case Karem::AppType::GameEngine:
				s_Application = new KaremEditor();
				break;
		}
	}

	KaremEditor::KaremEditor()
		: Application({ "Sandbox Application", 1280, 720 })
	{
		Init();
		m_Layers.PushLayer(std::make_shared<KaremEditorLayer>());
	}

	KaremEditor::KaremEditor(const WindowProperty& prop)
		: Application(prop)
	{
		Init();
		m_Layers.PushLayer(std::make_shared<KaremEditorLayer>());
	}

}