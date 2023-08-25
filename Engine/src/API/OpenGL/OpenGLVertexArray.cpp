#include "Core/Kareeem.h"

#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Karem {

	std::shared_ptr<VertexArray> CreateVertexArray()
	{
		return std::make_shared<OpenGLVertexArray>();
	}

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
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::Init()
	{
		glGenVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Clear()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

}