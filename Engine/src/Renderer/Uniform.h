#pragma once

#include "Core/Kareeem.h"

namespace Karem {

	struct Uniform
	{
	public:
		uint32_t Type;
		int32_t Location;
		uint32_t Count;
		void* Data;

		Uniform() = default;
		Uniform(uint32_t type, int32_t location, uint32_t count = 1)
			: Type(type), Location(location), Count(count), Data(nullptr) {}
	};
}