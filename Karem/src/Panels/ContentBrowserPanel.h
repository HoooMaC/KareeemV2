#pragma once

#include <filesystem>

#include "Renderer/SubTexture.h"

namespace Karem {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void Render();

	private:
		std::filesystem::path m_CurrentPath;
	};

}