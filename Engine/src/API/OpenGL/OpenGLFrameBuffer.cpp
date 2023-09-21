#include "Core/Kareeem.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Karem {

	OpenGLFrameBuffer::OpenGLFrameBuffer(int32_t width, int32_t height)
		: m_Width(width), m_Height(height)
	{
		// Buat framebuffer
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		// Buat texture warna terlampir
		glGenTextures(1, &m_TextureColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_TextureColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorAttachment, 0);

		// Periksa keberhasilan pembuatan framebuffer
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			// Handle kesalahan
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_TextureColorAttachment);
			m_RendererID = 0;
			m_TextureColorAttachment = 0;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_TextureColorAttachment);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Width, m_Height);
	}

	void OpenGLFrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(int32_t width, int32_t height)
	{
		m_Width = width;
		m_Height = height;

		glBindTexture(GL_TEXTURE_2D, m_TextureColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	
	}

}