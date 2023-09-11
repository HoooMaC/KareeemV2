#pragma once

#include "Renderer/BaseBuffer.h"

namespace Karem {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint64_t count);
		OpenGLIndexBuffer(void* data, uint64_t count);
		~OpenGLIndexBuffer();

		void Clear() override;

		void Bind() const override;
		void UnBind() const override;

	private:
		uint32_t m_RendererID;
	};	

}