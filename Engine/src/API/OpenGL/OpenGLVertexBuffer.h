#pragma once

#include "Renderer/BaseBuffer.h"
#include "Renderer/BufferLayout.h"

namespace Karem {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* data, size_t size);
		~OpenGLVertexBuffer();

		void Init(void* data, size_t size) override;
		void Clear() override;

		void ApplyLayout() override;
		void SetLayout(BufferLayout layout) override { m_Layout = layout; }
		BufferLayout GetLayout() const override { return m_Layout; }

		void Bind() const override;
		void UnBind() const override;

	private:
		BufferLayout m_Layout;
		uint32_t m_RendererID;

	};



}
