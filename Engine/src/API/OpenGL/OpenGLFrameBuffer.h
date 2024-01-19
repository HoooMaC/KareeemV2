#pragma once

#include "Renderer/FrameBuffer.h"

namespace Karem {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(FrameBufferSpecifications specifications);
		~OpenGLFrameBuffer();

		void Bind() const override;	
		void UnBind() const override;

		uint32_t GetTextureColorAttachmentID(uint32_t index = 0) const override
		{
			ENGINE_ASSERT(index < m_ColorAttachments.size(), "INVALID COLOR ATTACHMENT INDEX");
			return m_ColorAttachments[index];
		}
		float GetFramebufferWidth() const override { return (float)m_Specifications.Width; }
		float GetFramebufferHeight() const override { return (float)m_Specifications.Height; }

		void Resize(int32_t width, int32_t height) override;
		void Invalidate() override;

	private:
		uint32_t m_RendererID;

		FrameBufferSpecifications m_Specifications;

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification;
	
		std::vector<uint32_t>  m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};

}
