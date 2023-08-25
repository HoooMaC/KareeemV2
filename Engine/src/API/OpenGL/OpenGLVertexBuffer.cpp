#include "Core/Kareeem.h"

#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Karem {

	std::shared_ptr<VertexBuffer> CreateVertexBuffer(void* data, size_t size)
	{
		return std::make_shared<OpenGLVertexBuffer>(data, size);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, size_t size)
	{
		Init(data, size);
	}

	void OpenGLVertexBuffer::Init(void* data, size_t size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::Clear()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::UnBind() const
	{

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}