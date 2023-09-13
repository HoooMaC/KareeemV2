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
		glGenVertexArrays(1, &m_RendererID);
		Bind();
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

	void OpenGLVertexArray::UpdateBufferData(void* vbData, void* ibData, uint64_t size, uint64_t count) const
	{
		// for now only support modifying or upadating entire buffer data
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, vbData);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), ibData);
	}

	void OpenGLVertexArray::ApplyShaderLayout(BufferLayout& layout) const
	{
		for (const auto& [location, element] : layout)
		{
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(
				location,
				element.GetCount(),
				OpenGLToShaderDataType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
		}
	}

	void OpenGLVertexArray::Clear()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

}