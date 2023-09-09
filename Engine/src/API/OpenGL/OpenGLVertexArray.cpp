#include "Core/Kareeem.h"

#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Karem {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		Init();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		Clear();
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
		ENGINE_WARN("Binding vertex array with id [{}]", m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		//glBindVertexArray(m_RendererID);
		Bind();
		m_VertexBufferContainer.emplace_back(vertexBuffer);
		vertexBuffer->Bind();
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::Init()
	{
		glGenVertexArrays(1, &m_RendererID);
		//glBindVertexArray(m_RendererID);
		Bind();
	}

	void OpenGLVertexArray::Clear()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

}