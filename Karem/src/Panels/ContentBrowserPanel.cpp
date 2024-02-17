#include "Core/Kareeem.h"

#include "ContentBrowserPanel.h"

#include "Env/Color.h"

#include <imgui.h>

namespace Karem {

	static std::filesystem::path s_ParentDirectory = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentPath(s_ParentDirectory)
	{
		m_DirectoryIcon = CreateTexture2D("res/texture/icons/directory_icon.png");
		m_FileIcon = CreateTexture2D("res/texture/icons/directory_icon.png");
	}

	void ContentBrowserPanel::Render()
	{
		ImGui::Begin("ContentBrowserPanel");

		if (m_CurrentPath != s_ParentDirectory)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 120.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionMax().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		//ImGui::PushStyleColor(ImGuiCol_Button, HexToVec4<ImVec4>(Color::BlueButton, 0.0f));
		//ImGui::PushStyleColor(ImGuiCol_Button, HexToVec4<ImVec4>(Color::BlueButtonHovered, 0.5f));
		//ImGui::PushStyleColor(ImGuiCol_Button, HexToVec4<ImVec4>(Color::BlueButtonActive, 0.2f));

		for (auto directoryEntry : std::filesystem::directory_iterator{ m_CurrentPath })
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_ParentDirectory);
			std::string filenameString = relativePath.filename().string();

			std::shared_ptr<Texture2D> buttonIcon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::ImageButton((ImTextureID)buttonIcon->GetTextureID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });
			if (ImGui::IsItemHovered() and ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_CurrentPath /= path.filename();
				}
			}
			ImGui::TextWrapped(filenameString.c_str());
			ImGui::NextColumn();
		}
		
		ImGui::Columns(1);
		//ImGui::PopStyleColor(3);

		//ImGui::NewLine();
		//TODO::Give the value for the min max and speed
		ImGui::DragFloat("Button Size", &thumbnailSize, 5.0f, 50.0f, 200.0f, "%.f");
		ImGui::DragFloat("Padding", &padding, 1.0f, 10.0f, 32.0f, "%.f");

		ImGui::End();
	}

}