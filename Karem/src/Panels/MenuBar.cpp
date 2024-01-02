#include "Core/Kareeem.h"

#include "Menubar.h"
#include "SceneHierarcyPanel.h"
#include "Panel.h"

#include "Platform/Utils/FileDialog.h"

#include <imgui.h>
#include "external/imgui/imgui_configuration.h"

namespace Karem {

	void MenubarPanel::Render(bool* statusArray)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl + N"))
				{
					newSceneFunction();
				}
				if (ImGui::MenuItem("Save As...", "Ctrl + Shift + S"))
				{
					std::string filepath = FileDialog::SaveFile("Karem");
					saveSceneAsFunction(filepath);
				}
				if (ImGui::MenuItem("Open...", "Ctrl + O"))
				{
					std::string filepath = FileDialog::OpenFile("Karem");
					openSceneFunction(filepath);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Entity"))
			{
				ImGui::MenuItem("Entity List", nullptr, &statusArray[(int16_t)SceneHierarcyPanel::PanelsStatus::EntityList]);
				ImGui::MenuItem("Entity Component", nullptr, &statusArray[(int16_t)SceneHierarcyPanel::PanelsStatus::EntityComponent]);
				ImGui::MenuItem("Camera", nullptr, &statusArray[(int16_t)SceneHierarcyPanel::PanelsStatus::CameraPanel]);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Themes"))
			{
				if (ImGui::MenuItem("Karem Default"))
				{
					SetupKaremStyleDefault();
					KaremColorStyleDefault();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void MenubarPanel::EventHandler(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&MenubarPanel::KeyPressedAction, this, std::placeholders::_1));
	}

	bool MenubarPanel::KeyPressedAction(KeyPressedEvent& event)
	{
		bool isCtrlClicked = Input::IsKeyPressed(Key::LeftControl);
		bool isShiftClicked = Input::IsKeyPressed(Key::LeftShift);

		switch (event.GetKeyCode())
		{
			case Key::N:
			{
				if (isCtrlClicked)
				{
					newSceneFunction();
				}
				break;
			}
			case Key::S:
			{
				if (isCtrlClicked and isShiftClicked)
				{
					std::string filepath = FileDialog::SaveFile("Karem");
					saveSceneAsFunction(filepath);
				}
				break;
			}
			case Key::O:
			{
				if (isCtrlClicked)
				{
					std::string filepath = FileDialog::OpenFile("Karem");
					if(!filepath.empty())
						openSceneFunction(filepath);
				}
				break;
			}
		}
		return false;
	}
}
