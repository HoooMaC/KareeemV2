#pragma once

#include "Core/Kareeem.h"

namespace Karem {

	enum class ShaderDataType
	{
		None = 0, Float, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:
				return 0;
			case ShaderDataType::Float:
				return 4;
			case ShaderDataType::Vec2:
				return 4 * 2;
			case ShaderDataType::Vec3:
				return 4 * 3;
			case ShaderDataType::Vec4:
				return 4 * 4;
			case ShaderDataType::Mat2:
				return 4 * 2 * 2;
			case ShaderDataType::Mat3:
				return 4 * 3 * 3;
			case ShaderDataType::Mat4:
				return 4 * 4 * 4;
			case ShaderDataType::Int:
				return 4;
			case ShaderDataType::Int2:
				return 4 * 2;
			case ShaderDataType::Int3:
				return 4 * 3;
			case ShaderDataType::Int4:
				return 4 * 4;
			case ShaderDataType::Bool:
				return 1;
		}
		// TODO : NEED MAKE SOME ASSERTION HERE
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		uint32_t Size;
		ShaderDataType Type;
		bool Normalized;
		uint32_t Offset;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized =  false)
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
			return 0;
		}
	};
	
	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& layoutElement)
			: m_Elements(layoutElement)
		{
			CalculateOffsetAndStride();

		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

	private:
		inline void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			uint32_t index= 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;

			}

		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

}
