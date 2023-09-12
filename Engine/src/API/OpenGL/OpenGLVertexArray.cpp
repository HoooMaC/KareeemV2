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
		// this is temporary, Bind temp just for testing
		//Bind();
		// Cetak data vbData ke konsol
		float* vertexData = static_cast<float*>(vbData);
		for (uint64_t i = 0; i < size / sizeof(float); i += 10)
		{
			std::cout << vertexData[i] << " | " << vertexData[i+1] << " | " << vertexData[i+2] << " : " << vertexData[i+3] << " | " << vertexData[i+4] << " | "
				<< vertexData[i+5] << " | " << vertexData[i+6] << " : " << vertexData[i+7] << " | " << vertexData[i+8] << " | " << vertexData[9] << " |\n ";
		}
		std::cout << std::endl;

		//__debugbreak();
		// for now only support modifying or upadating entire buffer data
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, vbData);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), ibData);
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

	void OpenGLVertexArray::Clear()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

}