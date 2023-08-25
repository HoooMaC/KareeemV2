#pragma once

#include "Core/Renderer/BufferBase.h"

namespace Karem {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* data, size_t size);

		void Init(void* data, size_t size) override;
		void Clear() override;

		void Bind() const override;
		void UnBind() const override;
	private:
		uint32_t m_RendererID;
	};



}
