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

		inline void SetUniformList(const std::unordered_map<std::string, UniformElement>& uniformList)
		{
			m_UnifomList = uniformList;
		}

		// WARNING : Consider the lifetime of the object
		void SetUniformData(const std::string& name, void* data);

		// Sending the data to the GPUD
		// Consider for changing the name of the variable
		void UploadUniformData(const UniformElement& uniform);

		void Validate();
	private:
		std::unordered_map<std::string, UniformElement> m_UnifomList;
	};
}