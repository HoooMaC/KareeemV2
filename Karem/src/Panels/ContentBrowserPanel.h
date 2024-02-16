#pragma once

#include <filesystem>

#include "Renderer/BaseTexture.h"

namespace Karem {

	static std::filesystem::path s_ParentDirectory = "C:\\Code\\C++\\Karem\\Karem";

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void Render();

	private:
		std::filesystem::path m_CurrentPath = s_ParentDirectory;
	private:
		std::shared_ptr<Texture2D> m_DirectoryTexture;
	};

}