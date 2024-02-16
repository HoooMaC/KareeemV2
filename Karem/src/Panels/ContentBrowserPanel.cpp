#include "Core/Kareeem.h"

#include "ContentBrowserPanel.h"

#include "Env/Color.h"

#include <imgui.h>

namespace Karem {

	ContentBrowserPanel::ContentBrowserPanel()
	{
		m_DirectoryTexture = CreateTexture2D("C:\\Code\\C++\\Karem\\Karem\\res\\texture\\icons\\directory_icon.png", 1);
	}

	void ContentBrowserPanel::Render()
	{
		ImGui::Begin("ContentBrowserPanel");
		m_DirectoryTexture->Bind();
		if (m_CurrentPath != s_ParentDirectory)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}

		for (auto p : std::filesystem::directory_iterator{ m_CurrentPath })
		{
			std::string item = p.path().stem().string();
			if(p.is_directory())
			{
				uint64_t textureID = m_DirectoryTexture->GetTextureID();
				if(ImGui::ImageButton(reinterpret_cast<void*>(textureID), {100.0f, 100.0f}))
					m_CurrentPath = p.path();
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, HexToVec4<ImVec4>(Color::Blue, 1.0f));
				ImGui::Text(item.c_str());
				ImGui::PopStyleColor();
			}
		}
		m_DirectoryTexture->UnBind();
		ImGui::End();
	}

}