#include "MenuBar.h"

#include <imgui.h>
#include "external/imgui/imgui_configuration.h"

namespace Karem {

	std::vector<bool> MenuBar::panelStatus = std::vector<bool>(2);

	bool MenuBar::showEntityList = true;
	bool MenuBar::showEntityComponent = true;

	void MenuBar::Render()
	{
		// Begin menu bar +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::MenuItem("(demo menu)", NULL, false, false);
				if (ImGui::MenuItem("New")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}
				if (ImGui::BeginMenu("Open Recent"))
				{
					ImGui::MenuItem("fish_hat.c");
					ImGui::MenuItem("fish_hat.inl");
					ImGui::MenuItem("fish_hat.h");
					if (ImGui::BeginMenu("More.."))
					{
						ImGui::MenuItem("Hello");
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				if (ImGui::MenuItem("Save As..")) {}

				ImGui::Separator();
				if (ImGui::BeginMenu("Options"))
				{
					static bool enabled = true;
					ImGui::MenuItem("Enabled", "", &enabled);
					ImGui::BeginChild("child", ImVec2(0, 60), true);
					for (int i = 0; i < 10; i++)
						ImGui::Text("Scrolling Text %d", i);
					ImGui::EndChild();
					static float f = 0.5f;
					static int n = 0;
					ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
					ImGui::InputFloat("Input", &f, 0.1f);
					ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Colors"))
				{
					float sz = ImGui::GetTextLineHeight();
					for (int i = 0; i < ImGuiCol_COUNT; i++)
					{
						const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
						ImVec2 p = ImGui::GetCursorScreenPos();
						ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
						ImGui::Dummy(ImVec2(sz, sz));
						ImGui::SameLine();
						ImGui::MenuItem(name);
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Options"))
				{
					static bool b = true;
					ImGui::Checkbox("SomeOption", &b);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Disabled", false)) // Disabled
				{
					IM_ASSERT(0);
				}
				if (ImGui::MenuItem("Checked", NULL, true)) {}
				ImGui::Separator();
				ImGui::MenuItem("Quit", "Alt+F4");
				// close window here

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Examples"))
			{
				ImGui::MenuItem("Main menu bar");
				ImGui::MenuItem("Console");
				ImGui::MenuItem("Log");
				ImGui::MenuItem("Simple layout");
				ImGui::MenuItem("Property editor");
				ImGui::MenuItem("Long text display");
				ImGui::MenuItem("Auto-resizing window");
				ImGui::MenuItem("Constrained-resizing window");
				ImGui::MenuItem("Simple overlay");
				ImGui::MenuItem("Fullscreen window");
				ImGui::MenuItem("Manipulating window titles");
				ImGui::MenuItem("Custom rendering");
				ImGui::MenuItem("Dockspace");
				ImGui::MenuItem("Documents");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Entity"))
			{
				ImGui::MenuItem("Entity List", NULL, &showEntityList);
				ImGui::MenuItem("Entity Component", NULL, &showEntityComponent);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Theme"))
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
				ImGui::MenuItem("Metrics/Debugger");
				ImGui::MenuItem("Debug Log");
				ImGui::MenuItem("Stack Tool");
				ImGui::MenuItem("Style Editor");
				ImGui::MenuItem("About Dear ImGui");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		// End menu bar +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}

}
