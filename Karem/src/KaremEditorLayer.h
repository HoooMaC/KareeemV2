#pragma once

#include "KaremEngine.h"

#include "Panels/Panel.h"

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

	private:
		void RenderDockspace();
		void RenderViewportPanel();
		void ChangeScene() {} // TODO : Need to implemented
	private:
		bool WindowResizeAction(WindowResizeEvent& event);
		bool KeyPressedAction(KeyPressedEvent& event);
		bool MouseButtonAction(MouseButtonPressedEvent& event);

		void NewScene()
		{
			m_ActiveScene = std::make_shared<Scene>();
			m_Panels.SetScene(m_ActiveScene);
		}

		void SaveSceneAs(std::string_view filepath)
		{
			SceneSerializer::Serialize(m_ActiveScene, filepath);
		}

		void OpenScene(std::string_view filepath)
		{
			m_ActiveScene.reset(new Scene);
			SceneSerializer::Deserialize(m_ActiveScene, filepath);
			m_Panels.SetScene(m_ActiveScene);
		}
	private:
		enum class State
		{
			Edit = 0,
			Play
		};
		State m_CurrentState = State::Edit;
		uint32_t m_CurrentGizmoType = -1;
	private:
		glm::vec2 m_CurrentViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_PreviousViewportSize = { 0.0f, 0.0f };

		Panels m_Panels;

		std::shared_ptr<FrameBuffer> m_FrameBuffer; // who owns the frame buffer
		std::shared_ptr<Scene> m_ActiveScene;

		EditorCamera m_EditorCamera;
	};

}