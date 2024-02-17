#pragma once

#include <filesystem>

#include "Renderer/BaseTexture.h"

namespace Karem {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void Render();

	private:
		std::shared_ptr<Texture2D> m_DirectoryIcon, m_FileIcon;
		std::filesystem::path m_CurrentPath;
	};

}