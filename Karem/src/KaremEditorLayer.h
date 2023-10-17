#pragma once

#include "KaremEngine.h"

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
		Scene m_ActiveScene;
		OrthographicCamera m_Camera;

		std::shared_ptr<Texture2D> m_Texture;
		std::shared_ptr<FrameBuffer> m_FrameBuffer;
		SubTexture2D m_SpriteSheet;
	};

}