#include "Core/Kareeem.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Karem {

	namespace Utils {
		
		static GLenum TextureTarget(bool isMultisampled)
		{
			return isMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTexture(bool isMultisampled, uint32_t* outId, uint32_t count)
		{
			glCreateTextures(TextureTarget(isMultisampled), count, outId);
		}

		static void BindTexture(bool isMultisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(isMultisampled), id);
		}

		static void AttachColorTexture(uint32_t id, uint32_t samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool isMultisampled = samples > 1;
			if (isMultisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(isMultisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, uint32_t samples, GLenum internalFormat, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool isMultiSampled = samples > 1;
			if (isMultiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(isMultiSampled), id, 0);
		}
				
		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case Karem::FrameBufferTextureFormat::DEPTH24STENCIL8:
					return true;
			}
			return false;
		}

		static GLenum TextureFormatFBToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case Karem::FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
				case Karem::FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}
			ENGINE_ASSERT(false, "Invalid format");
			return 0;
		}

	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferSpecifications specifications)
		: m_Specifications(specifications)
	{
		for (const auto& spec : m_Specifications.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
			{
				m_ColorAttachmentSpecifications.emplace_back(spec);
			}
			else
			{
				m_DepthAttachmentSpecification = spec;
			}
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specifications.Width, m_Specifications.Height);
	}

	void OpenGLFrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::ClearColorAttachment(uint32_t attachmentIndex, int value)
	{
		ENGINE_ASSERT(!(attachmentIndex < m_ColorAttachments.size()), "Invalid color attachment");

		auto spec = m_ColorAttachmentSpecifications[attachmentIndex].TextureFormat;
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::TextureFormatFBToGL(spec), GL_INT, &value);
	}

	int OpenGLFrameBuffer::GetEntityId(uint32_t attachmentIndex, uint32_t x, uint32_t y) const
	{
		ENGINE_ASSERT(!(attachmentIndex < m_ColorAttachments.size()), "Invalid color attachment");

		int pixelData;
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::Resize(int32_t width, int32_t height)
	{
		if (width <= 0 || height <= 0)
		{
			ENGINE_ASSERT(false, "");
			// Nilai tidak valid, kembalikan atau tangani kesalahan sesuai kebutuhan Anda
			return;
		}

		m_Specifications.Width = width;
		m_Specifications.Height = height;

		Invalidate();
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);


		bool isMultisampled = m_Specifications.Samples > 1;
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTexture(isMultisampled, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (int i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(isMultisampled, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
					case FrameBufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specifications.Samples, GL_RGBA8, GL_RGBA, m_Specifications.Width, m_Specifications.Height, i);
						break;
					case FrameBufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specifications.Samples, GL_R32I, GL_RED_INTEGER, m_Specifications.Width, m_Specifications.Height, i);
						break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None)
		{
			Utils::CreateTexture(isMultisampled, &m_DepthAttachment, 1);
			Utils::BindTexture(isMultisampled, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
				case FrameBufferTextureFormat::DEPTH24STENCIL8:
					Utils::AttachDepthTexture(m_DepthAttachment, m_Specifications.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specifications.Width, m_Specifications.Height);
						break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			ENGINE_ASSERT(!(m_ColorAttachments.size() <= 4), "Only supports 4 color attachments");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if(m_ColorAttachments.empty())
		{
			// Only depth pass
			glDrawBuffer(GL_NONE);
		}

#if 0
		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_TextureColorAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

#elif OLD_CODE
		// Build the texture that will serve as the depth attachment for the framebuffer.
		GLuint depthTexture;
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenTextures(1, &m_TextureColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_TextureColorAttachment);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorAttachment, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		m_DepthAttachment = depthTexture;

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
#endif

		ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}