#pragma once

#include "Renderer/FrameBuffer.h"

namespace Karem {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(int32_t width, int32_t height);
		~OpenGLFrameBuffer();

		void Bind() const override;
		void UnBind() const override;

		uint32_t GetTextureColorAttachmentID() const { return m_TextureColorAttachment; }

		void Resize(int32_t width, int32_t height) override;
	private:
		uint32_t m_RendererID;
		uint32_t m_TextureColorAttachment;
		uint32_t m_DepthAttachment;

		int32_t m_Width, m_Height;

	};

}
