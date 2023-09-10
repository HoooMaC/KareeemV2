#pragma once

#include "Core/Kareeem.h"

#include <unordered_map>
#include <string>

namespace Karem {

	struct UniformElement
	{
	public:
		uint32_t Type;
		int32_t Location;
		uint32_t Count;
		void* Data;

		UniformElement() = default;
		UniformElement(uint32_t type, int32_t location, uint32_t count = 1)
			: Type(type), Location(location), Count(count), Data(nullptr) {}
	};

	class UniformCache
	{
	public:	
		UniformCache() = default;
		UniformCache(std::unordered_map<std::string, UniformElement>& uniformList)
			: m_UnifomList(uniformList) {}

		inline void SetUniform(const std::unordered_map<std::string, UniformElement>& uniformList)
		{
			m_UnifomList = uniformList;
		}

		inline void Validate()
		{
			for (auto [name, uniform] : m_UnifomList)
			{
				if (uniform.Data == nullptr)
				{
					ENGINE_ASSERT(false, "Unitilialized Uniform");
				}
			}
		}
	private:
		std::unordered_map<std::string, UniformElement> m_UnifomList;
	};
}