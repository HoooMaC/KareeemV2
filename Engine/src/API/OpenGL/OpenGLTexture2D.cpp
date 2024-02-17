#include "Core/Kareeem.h"

#include "OpenGLTexture2D.h"

#include <stb_image/stb_image.h>
#include <glad/glad.h>

namespace Karem {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t slot)
		: m_FilePath(""), m_Slot(slot), m_Width(0), m_Height(0), m_BPP(0), m_RendererID(0)
	{
		uint32_t format = GL_RGBA;
		unsigned char whitePixel[] = { 255, 255, 255, 255 }; // Putih dengan alpha penuh
		m_Width = 1;
		m_Height = 1;
		m_BPP = 4;
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//ENGINE_DEBUG("White texture ID {}", m_RendererID);

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, whitePixel);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	OpenGLTexture2D::OpenGLTexture2D(std::string_view filePath, uint32_t slot, uint32_t filter)
		: m_FilePath(filePath), m_Slot(slot), m_Width(0), m_Height(0), m_BPP(0), m_RendererID(0)
	{
		CreateTexture(filter);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ClearTexture();
	}

	void OpenGLTexture2D::Bind() const
	{
		glActiveTexture(GL_TEXTURE0 + m_Slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture2D::UnBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::CreateTexture(uint32_t filter)
	{	
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 0);
		if (data)
		{
			uint32_t format = (m_BPP == 3) ? GL_RGB : GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else 
		{
			ENGINE_ASSERT(true, "Failed to load image : {}", m_FilePath);
		}
		stbi_image_free(data);
	}

	void OpenGLTexture2D::ClearTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

}