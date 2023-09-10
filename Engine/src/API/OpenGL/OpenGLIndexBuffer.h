#pragma once

#include "Renderer/BaseBuffer.h"

namespace Karem {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(void* data, uint64_t count);
		~OpenGLIndexBuffer();

		void Init(void* data, uint64_t count) override;
		void Clear() override;

		void Bind() const override;
		void UnBind() const override;

	private:
		uint32_t m_RendererID;
	};	

}