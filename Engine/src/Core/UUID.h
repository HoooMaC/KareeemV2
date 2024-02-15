#pragma once

#include <stdint.h>

namespace Karem {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t id);

		UUID(const UUID&) = default;

		operator uint64_t() const { return ID; }
	private:
		uint64_t ID;
	};

}
