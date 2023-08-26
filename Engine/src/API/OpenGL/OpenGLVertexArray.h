#pragma once

#include "Core/Renderer/BaseBuffer.h"

#include <vector>

namespace Karem {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Init();
		void Clear();

		void Bind() const;
		void UnBind() const;


		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBufferContainer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
