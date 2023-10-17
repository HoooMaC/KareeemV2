#include "KaremAppEditor.h"

namespace Karem {

	Application* CreateApplication()
	{
		return new KaremEditor();
	}

	KaremEditor::KaremEditor()
		: Application({ "Sandbox Application", 1280, 720 })
	{
		Init();
		m_Layers.PushLayer(std::make_shared<KaremEditorLayer>());
	}

}