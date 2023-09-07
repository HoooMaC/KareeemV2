#include "Core/Kareeem.h"

#include "OpenGLTexture2D.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace Karem {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath, uint32_t slot)
		: m_FilePath(filePath), m_Slot(slot), m_Width(0), m_Height(0), m_BPP(0), m_RendererID(0)
	{
		CreateTexture();
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ClearTexture();
	}

	void OpenGLTexture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture2D::UnBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::CreateTexture()
	{	
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
			ENGINE_ERROR("Failed to load image : {}", m_FilePath);
		}
		stbi_image_free(data);
	}

	void OpenGLTexture2D::ClearTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

}