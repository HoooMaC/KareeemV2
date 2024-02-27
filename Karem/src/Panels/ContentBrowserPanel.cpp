#include "Core/Kareeem.h"

#include "ContentBrowserPanel.h"

#include "Env/Color.h"

#include "Renderer/SubTexture.h"
#include "Renderer/SmartTexture.h"

#include <imgui.h>

namespace Karem {

	static std::filesystem::path s_ParentDirectory = "assets";
	//static std::shared_ptr<Texture2D> s_DirectoryIcons;
	//static std::shared_ptr<Texture2D> s_SceneIcons;

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentPath(s_ParentDirectory), m_ButtonIcons("res/button_icons/first.png", 512, 512, 1, 7)
	{
		//s_DirectoryIcons = CreateTexture2D("res/texture/icons/directory_icon.png", 1);
		//s_SceneIcons = CreateTexture2D("res/texture/icons/scene_icon.png", 2);
	}

	void ContentBrowserPanel::Render()
	{
		ImGui::Begin("ContentBrowserPanel");
		//s_SceneIcons->Bind();
		//s_DirectoryIcons->Bind();
		//m_ButtonIcons.Bind();

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

			auto textureID = m_ButtonIcons.GetTextureID();
			std::array<glm::vec2, 2> texCoord;
			if(directoryEntry.is_directory())
				texCoord = m_ButtonIcons.GetTexCoord(3, 0, 1, 1);
			else if(directoryEntry.path().extension() == std::filesystem::path(".karem"))
				texCoord = m_ButtonIcons.GetTexCoord(4, 0, 1, 1);
			else if(directoryEntry.path().extension() == std::filesystem::path(".png"))
				texCoord = m_ButtonIcons.GetTexCoord(5, 0, 1, 1);
			// TODO::The default icon still same with directory icon
			else
				texCoord = m_ButtonIcons.GetTexCoord(3, 0, 1, 1);

			ImVec2 uv0{ texCoord[0].x, texCoord[0].y };
			ImVec2 uv1{ texCoord[1].x, texCoord[1].y };

			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToVec4<ImVec4>(Color::AntiqueWhite, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToVec4<ImVec4>(Color::White, 0.7f));

			ImGui::PushID(filenameString.c_str());

			ImGui::ImageButton((ImTextureID)textureID, { thumbnailSize, thumbnailSize }, uv0, uv1);
			//ImGui::ImageButton((ImTextureID)textureID, { thumbnailSize, thumbnailSize }, {0,1},{1,0});
			if (ImGui::BeginDragDropSource())
			{
				std::string payloadSource = path.string();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", payloadSource.c_str(), payloadSource.length() + 1, ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopID();

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
		//ImGui::PopStyleColor(3);

		//ImGui::NewLine();
		//TODO::Give the value for the min max and speed
		ImGui::DragFloat("Button Size", &thumbnailSize, 5.0f, 50.0f, 200.0f, "%.f");
		ImGui::DragFloat("Padding", &padding);

		ImGui::End();
	}

}