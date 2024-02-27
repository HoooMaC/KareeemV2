#pragma once

#include <filesystem>

#include "Renderer/SmartTexture.h"

namespace Karem {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void Render();

	private:
		SmartTexture m_ButtonIcons;
	private:
		std::filesystem::path m_CurrentPath;
	};

}