#pragma once


#include "Renderer/SmartTexture.h"

#include <Rahman/SmartRef.h>

#include <filesystem>

namespace Karem {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void Render();

	private:
		Rahman::SmartRef<SmartTexture> m_ButtonIcons;
	private:
		std::filesystem::path m_CurrentPath;
	};

}