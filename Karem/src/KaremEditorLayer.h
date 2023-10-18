#pragma once

#include "KaremEngine.h"

#include "Panels/SceneHierarcyPanel.h"

namespace Karem {

	class KaremEditorLayer : public Layer
	{
	public:
		KaremEditorLayer()
			: Layer("Sandbox Layer") {}

		void Update(TimeStep ts) override;

		void OnAttach() override;
		void OnDetach() override;

		void RenderImGUI() override;
		void EventHandler(Event& event) override;

		// TO DO : this is shold close the window. For now this do nothing
		void CloseWindow() {}

	private:
		bool WindowResizeAction(WindowResizeEvent& event);

	private:
		OrthographicCamera m_Camera;
		SceneHierarcyPanel m_HierarcyPanel;
			
		std::shared_ptr<Scene> m_ActiveScene;
		std::shared_ptr<Texture2D> m_Texture;
		SubTexture2D m_SpriteSheet;
	};

}