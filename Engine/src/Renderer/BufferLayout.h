#pragma once

#include "Core/Kareeem.h"
#include "Renderer/BaseShader.h"

// this is temporary ABSOLUTELY!!!
#include <glad/glad.h>

namespace Karem {

	static ShaderDataType UintToShaderDataType(uint32_t type)
	{
		switch (type)
		{
			case GL_FLOAT:			return ShaderDataType::Float;
			case GL_FLOAT_VEC2:		return ShaderDataType::Vec2;
			case GL_FLOAT_VEC3:		return ShaderDataType::Vec3;
			case GL_FLOAT_VEC4:		return ShaderDataType::Vec4;
			case GL_FLOAT_MAT2:		return ShaderDataType::Mat2;
			case GL_FLOAT_MAT3:		return ShaderDataType::Mat3;
			case GL_FLOAT_MAT4:		return ShaderDataType::Mat4;
			case GL_INT:			return ShaderDataType::Int;
			case GL_INT_VEC2:		return ShaderDataType::Int2;
			case GL_INT_VEC3:		return ShaderDataType::Int3;
			case GL_INT_VEC4:		return ShaderDataType::Int4;
		}
		ENGINE_ASSERT(false, "UNSUPPORTED DATA TYPE");
		return ShaderDataType::None;
	}

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:return 4;
			case ShaderDataType::Vec2: return 4 * 2;
			case ShaderDataType::Vec3: return 4 * 3;
			case ShaderDataType::Vec4: return 4 * 4;
			case ShaderDataType::Mat2: return 4 * 2 * 2;
			case ShaderDataType::Mat3: return 4 * 3 * 3;
			case ShaderDataType::Mat4: return 4 * 4 * 4;
			case ShaderDataType::Int:  return 4;
			case ShaderDataType::Int2: return 4 * 2;
			case ShaderDataType::Int3: return 4 * 3;
			case ShaderDataType::Int4: return 4 * 4;
			case ShaderDataType::Bool: return 1;
		}
		ENGINE_ASSERT(false, "Invalid Shader Data Type");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		uint32_t Size;
		ShaderDataType Type;
		bool Normalized;
		uint32_t Offset;

		BufferElement() = default;
		BufferElement(ShaderDataType type, std::string_view name, bool normalized =  false)
			: Name(name), Size(ShaderDataTypeSize(type)), Type(type), Normalized(normalized), Offset(0) 
		{
		}

		uint32_t GetCount() const
		{
			switch (Type)

			{
				case ShaderDataType::None:
					return 0;
				case ShaderDataType::Float:
					return 1;
				case ShaderDataType::Vec2:
					return 2;
				case ShaderDataType::Vec3:
					return 3;
				case ShaderDataType::Vec4:
					return 4;
				case ShaderDataType::Mat2:
					return 2 * 2;
				case ShaderDataType::Mat3:
					return 3 * 3;
				case ShaderDataType::Mat4:
					return 4 * 4;
				case ShaderDataType::Int:
					return 1;
				case ShaderDataType::Int2:
					return 2;
				case ShaderDataType::Int3:
					return 3;
				case ShaderDataType::Int4:
					return 4;
				case ShaderDataType::Bool:
					return 1;
			}
			// TO DO : NEED SOME ASSERTION HERE	
			ENGINE_ASSERT(false, "Unsuppored Data Type");
			return 0;
		}
	};
	
	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::map<int32_t, BufferElement>& layoutElement)
			: m_Elements(layoutElement)
		{
			CalculateOffsetAndStride();
		}

		inline const std::map<int32_t, BufferElement>& GetElements() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }

		std::map<int32_t, BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::map<int32_t, BufferElement>::iterator end() { return m_Elements.end(); }

	private:
		inline void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			uint32_t index= 0;

			for (auto& [location, element] : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;

			}
		}
	private:
		std::map<int32_t, BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

}
