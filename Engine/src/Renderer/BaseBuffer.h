#pragma once

#include "Renderer/BufferLayout.h"

namespace Karem {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Clear() = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};

	std::shared_ptr<VertexBuffer> CreateVertexBuffer(size_t size);
	std::shared_ptr<VertexBuffer> CreateVertexBuffer(void* data, size_t size);

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Clear() = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};

	std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint64_t count);
	std::shared_ptr<IndexBuffer> CreateIndexBuffer(void* data, uint64_t count);

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Clear() = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void UpdateBufferData(void* vbData, void* ibData, uint64_t size, uint64_t count) const = 0;

		virtual void ApplyShaderLayout(BufferLayout& layout) const = 0;
	};

	std::shared_ptr<VertexArray> CreateVertexArray();


}
