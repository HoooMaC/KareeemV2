#include "Core/Kareeem.h"
#include "SmartTexture.h"

#include <glad/glad.h>

namespace Karem {

	extern std::unordered_map<std::string, uint32_t> g_Texture;

	SmartTexture::SmartTexture(const char* path, int cellWidth, int cellHeight, int textureRow, int textureCol)
		: m_CellWidth(cellWidth), m_CellHeight(cellHeight), m_TextureRow(textureRow), m_TextureCol(textureCol)
	{
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_set_flip_vertically_on_load(true);

		// !TEMP::we still don't use this bpp
		int bpp;
		unsigned char* data = stbi_load(path, &m_TextureWidth, &m_TextureHeight, &bpp, 0);
		if (data)
		{
			uint32_t format = (bpp == 3) ? GL_RGB : GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_TextureWidth, m_TextureHeight, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			ENGINE_ASSERT(true, "Failed to load image : {}", path);
		}
		stbi_image_free(data);
		
		std::hash<std::string> hasher;
		size_t hashValue = hasher(path + m_TextureID);

		g_Texture[std::to_string(hashValue)] = m_TextureID;
		//glActiveTexture(GL_TEXTURE0 + 1);
		//glBindTexture(GL_TEXTURE, m_TextureID);
	}

	SmartTexture::~SmartTexture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void SmartTexture::Bind() const
	{
		glActiveTexture(GL_TEXTURE0 + 10);
		glBindTexture(GL_TEXTURE, m_TextureID);
	}

	std::array<glm::vec2, 2> SmartTexture::GetTexCoord(int spriteX, int spriteY, int spriteWidth, int spriteHeight, bool isFlip) const
	{
		if (spriteX < 0 || spriteY < 0 || spriteX >= m_TextureCol || spriteY >= m_TextureRow)
		{
			ENGINE_ASSERT(false, "Sprite coordinates out of bounds");
			// You should handle the out of bounds case here.
			// For now, let's return some default values.
			return { glm::vec2(0.0f), glm::vec2(1.0f) };
		}

		glm::vec2 min = { (spriteX * (float)m_CellWidth) / m_TextureWidth, (spriteY * (float)m_CellHeight) / m_TextureHeight };
		glm::vec2 max = { ((spriteX + spriteWidth) * (float)m_CellWidth) / m_TextureWidth, ((spriteY + spriteHeight) * (float)m_CellHeight) / m_TextureHeight };

		if (isFlip)
		{
			std::swap(min.y, max.y);
		}

		return { min, max };
	}

}