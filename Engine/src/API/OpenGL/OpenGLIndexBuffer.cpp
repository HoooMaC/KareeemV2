#include "Core/Kareeem.h"

#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace Karem {

	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint64_t count)
	{
		Init(data, count);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		Clear();
	}

	void OpenGLIndexBuffer::Init(void* data, uint64_t count)
	{
		m_Indices = std::vector<uint32_t>((uint32_t*)data, (uint32_t*)data + count);

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	void OpenGLIndexBuffer::Clear()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}