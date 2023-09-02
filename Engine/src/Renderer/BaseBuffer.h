#pragma once

#include "Renderer/BufferLayout.h"

namespace Karem {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Init(void* data, size_t size) = 0;
		virtual void Clear() = 0;

		virtual void ApplyLayout() = 0;
		virtual void SetLayout(BufferLayout layout) = 0;
		virtual BufferLayout GetLayout() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

	};

	std::shared_ptr<VertexBuffer> CreateVertexBuffer(void* data, size_t size);

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Init(void* data, uint64_t count) = 0;
		virtual void Clear() = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual int32_t GetCount() const = 0;

	};

	std::shared_ptr<IndexBuffer> CreateIndexBuffer(void* data, uint64_t count);

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Init() = 0;
		virtual void Clear() = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
	};

	std::shared_ptr<VertexArray> CreateVertexArray();


}
