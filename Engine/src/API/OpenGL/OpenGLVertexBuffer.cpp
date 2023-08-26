#include "Core/Kareeem.h"

#include "OpenGLVertexBuffer.h"

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

	std::shared_ptr<VertexBuffer> CreateVertexBuffer(void* data, size_t size)
	{
		return std::make_shared<OpenGLVertexBuffer>(data, size);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, size_t size)
	{
		Init(data, size);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		Clear();
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

	void OpenGLVertexBuffer::ApplyLayout()
	{
		int32_t index = 0;
		for (auto& element : m_Layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetCount(),
				OpenGLToShaderDataType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				element.Size,
				(const void*)element.Offset
			);
			index++;
		}
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