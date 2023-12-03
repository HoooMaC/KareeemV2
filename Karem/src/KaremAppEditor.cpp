#include "KaremAppEditor.h"

namespace Karem {

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