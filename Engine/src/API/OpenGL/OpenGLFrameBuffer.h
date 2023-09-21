#pragma once

#include "Renderer/FrameBuffer.h"

namespace Karem {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(int32_t width, int32_t height);
		~OpenGLFrameBuffer();

		void Bind() const;
		void UnBind() const;

		void Resize(int32_t width, int32_t height);
	private:
		uint32_t m_RendererID;
		uint32_t m_TextureColorAttachment;

		int32_t m_Width, m_Height;

	};

}
