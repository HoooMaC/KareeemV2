#pragma once

#include <stb_image/stb_image.h>

namespace Karem {

	class SmartTexture
	{
	public:
		// cell size should be in pixel
		SmartTexture() = default;
		SmartTexture(const char* path, int cellWidth, int cellHeight, int textureRow, int textureColumn);
		~SmartTexture();

		void Bind() const;

		[[nodiscard]] const void* GetTextureID() const { return reinterpret_cast<const void*>(m_TextureID); }

		// TODO::FIX the return value type
		[[nodiscard]] std::array<glm::vec2, 2> GetTexCoord(int spriteX, int spriteY, int spriteWidth, int spriteHeight, bool isFlip = true) const;

	private:
		uint32_t m_TextureID = 0;

		int m_TextureWidth = 0, m_TextureHeight = 0;

		uint32_t m_TextureRow = 0, m_TextureCol = 0;
		uint32_t m_CellHeight = 0;
		uint32_t m_CellWidth = 0;
	};

}
