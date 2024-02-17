#include "Core/Kareeem.h"

#include "ContentBrowserPanel.h"

#include "Env/Color.h"

#include "Renderer/SubTexture.h"

#include <imgui.h>

namespace Karem {

	static std::filesystem::path s_ParentDirectory = "assets";
	static std::shared_ptr<Texture2D> s_DirectoryIcons;
	static std::shared_ptr<Texture2D> s_SceneIcons;

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentPath(s_ParentDirectory)
	{
		s_DirectoryIcons = CreateTexture2D("res/texture/icons/directory_icon.png");
		s_SceneIcons = CreateTexture2D("res/texture/icons/scene_icon.png");
	}

	void ContentBrowserPanel::Render()
	{
		ImGui::Begin("ContentBrowserPanel");
		s_SceneIcons->Bind();
		s_DirectoryIcons->Bind();
		
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

		for (auto directoryEntry : std::filesystem::directory_iterator{ m_CurrentPath })
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_ParentDirectory);
			std::string filenameString = relativePath.filename().string();

			auto textureID = directoryEntry.is_directory() ? s_DirectoryIcons->GetTextureID() : s_SceneIcons->GetTextureID();
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToVec4<ImVec4>(Color::AntiqueWhite, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToVec4<ImVec4>(Color::White, 0.7f));
			ImGui::ImageButton((ImTextureID)textureID, { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });
			ImGui::PopStyleColor(3);
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

		//TODO::Give the value for the min max and speed
		ImGui::DragFloat("Button Size", &thumbnailSize, 5.0f, 50.0f, 200.0f, "%.f");
		ImGui::DragFloat("Padding", &padding, 1.0f, 5.0f, 32.0f, "%.f");
		s_DirectoryIcons->UnBind();
		s_SceneIcons->UnBind();
		ImGui::End();
	}

}