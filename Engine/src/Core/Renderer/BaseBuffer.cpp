#include "Core/Kareeem.h"

#include "BaseBuffer.h"

#include "API/OpenGL/OpenGLVertexArray.h"
#include "API/OpenGL/OpenGLVertexBuffer.h"
#include "API/OpenGL/OpenGLIndexBuffer.h"

namespace Karem {

	std::shared_ptr<VertexArray> VertexArray::CreateVertexArray()
	{
		return std::make_shared<OpenGLVertexArray>();
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::CreateVertexBuffer(void* data, size_t size)
	{
		return std::make_shared<OpenGLVertexBuffer>(data, size);
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(void* data, uint64_t count)
	{
		return std::make_shared<OpenGLIndexBuffer>(data, count);
	}

}