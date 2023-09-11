#pragma once

#include "Renderer/BaseBuffer.h"

#include <vector>

namespace Karem {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Clear();

		void Bind() const;
		void UnBind() const;

		void UpdateBufferData(void* vbData, void* ibData, uint64_t size, uint64_t count) const override;

		void ApplyShaderLayout(BufferLayout& layout) const;
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferID, m_IndexBufferID;
	};

}
