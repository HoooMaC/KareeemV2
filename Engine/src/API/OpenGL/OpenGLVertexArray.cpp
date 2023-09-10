#include "Core/Kareeem.h"

#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Karem {

	GLenum OpenGLToShaderDataType(Karem::ShaderDataType type)
	{
		switch (type)

		{
		case Karem::ShaderDataType::None:
			return 0;
		case Karem::ShaderDataType::Float:
			return GL_FLOAT;
		case Karem::ShaderDataType::Vec2:
			return GL_FLOAT;
		case Karem::ShaderDataType::Vec3:
			return GL_FLOAT;
		case Karem::ShaderDataType::Vec4:
			return GL_FLOAT;
		case Karem::ShaderDataType::Mat2:
			return GL_FLOAT;
		case Karem::ShaderDataType::Mat3:
			return GL_FLOAT;
		case Karem::ShaderDataType::Mat4:
			return GL_FLOAT;
		case Karem::ShaderDataType::Int:
			return GL_INT;
		case Karem::ShaderDataType::Int2:
			return GL_INT;
		case Karem::ShaderDataType::Int3:
			return GL_INT;
		case Karem::ShaderDataType::Int4:
			return GL_INT;
		case Karem::ShaderDataType::Bool:
			return GL_BOOL;
		}
		// TO DO : NEED SOME ASSERTION HERE
		return 0;
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

	void OpenGLVertexArray::ApplyShaderLayout(BufferLayout& layout) const
	{
		int32_t index = 0;
		for (auto element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetCount(),
				OpenGLToShaderDataType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}
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