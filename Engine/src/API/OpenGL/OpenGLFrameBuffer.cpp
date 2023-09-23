#include "Core/Kareeem.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Karem {

	OpenGLFrameBuffer::OpenGLFrameBuffer(int32_t width, int32_t height)
		: m_RendererID(0), m_TextureColorAttachment(0), m_DepthAttachment(0), m_Width(width), m_Height(height)
	{
		glGenTextures(1, &m_TextureColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_TextureColorAttachment);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);

		// Build the texture that will serve as the depth attachment for the framebuffer.
		GLuint depth_texture;
		glGenTextures(1, &depth_texture);
		glBindTexture(GL_TEXTURE_2D, depth_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorAttachment, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		// Periksa keberhasilan pembuatan framebuffer
		ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame Buffer is incomplete");
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			// Handle kesalahan
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_TextureColorAttachment);

			m_RendererID = 0;
			m_TextureColorAttachment = 0;
			m_DepthAttachment = 0;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_DepthAttachment = depth_texture;
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_TextureColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
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