#pragma once

#include "Renderer/BaseTexture.h"

#include <glm/glm.hpp>

namespace Karem {

	class SubTexture2D
	{
	public:
		SubTexture2D(const std::shared_ptr<Texture2D>& reference, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = glm::vec2(1.0f))
			: m_TextureReference(reference)
		{
			float textureReferenceWidth = (float)reference->GetWidth();
			float textureReferenceHeight = (float)reference->GetHeight();

			glm::vec2 min = { (coords.x * cellSize.x) / textureReferenceWidth, (coords.y * cellSize.y) / textureReferenceHeight };
			glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / textureReferenceWidth, ((coords.y + spriteSize.y) * cellSize.y) / textureReferenceHeight };

			m_TexCoord[0] = { min.x, min.y };
			m_TexCoord[1] = { max.x, min.y };
			m_TexCoord[2] = { max.x, max.y };
			m_TexCoord[3] = { min.x, max.y };
		}

		//SubTexture2D(const std::shared_ptr<Texture2D>& reference, const glm::vec2& min, const glm::vec2& max) 
		//	: m_TextureReference(reference)
		//{
		//	m_TexCoord[0] = { min.x, min.y };
		//	m_TexCoord[1] = { max.x, min.y };
		//	m_TexCoord[2] = { max.x, max.y };
		//	m_TexCoord[3] = { min.x, max.y };
		//}

		const glm::vec2* GetTexCoord() const { return m_TexCoord; }
		const std::shared_ptr<Texture2D>& GetTextureReference() const { return m_TextureReference; }		
	private:
		const std::shared_ptr<Texture2D>& m_TextureReference;
		glm::vec2 m_TexCoord[4];
	};

	//inline std::shared_ptr<SubTexture2D> CreateSubTexture(const std::shared_ptr<Texture2D>& reference, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = glm::vec2(1.0f))
	//{
	//	float textureReferenceWidth = (float)reference->GetWidth();
	//	float textureReferenceHeight = (float)reference->GetHeight();
	//	glm::vec2 min = { (coords.x * cellSize.x) / textureReferenceWidth, (coords.y * cellSize.y) / textureReferenceHeight };
	//	glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / textureReferenceWidth, ((coords.y + spriteSize.y) * cellSize.y) / textureReferenceHeight };
	//	return std::make_shared<SubTexture2D>(reference, min, max);
	//}
}
