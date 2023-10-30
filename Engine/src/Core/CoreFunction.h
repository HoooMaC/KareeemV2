#pragma once

#include <glm/glm.hpp>
#include <string>
#include <regex>

#ifdef ACTIVATE_LOGGING
#include "DefinedLog/EngineLog.h"
#else
#include "UnactiveLog.h"
#endif

namespace Karem {

	template<class T>
	T HexToVec4(std::string_view hexColor)
	{
		std::regex hexPattern("^#?([0-9a-fA-F]{6}|[0-9a-fA-F]{8})$");
		if (!std::regex_match(hexColor.begin(), hexColor.end(), hexPattern))
		{
			ENGINE_ASSERT(false, "Invalid hex color format");
			return T(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// Tentukan indeks awal untuk melewati "#" jika ada
		size_t startIndex = 0;
		if (hexColor[0] == '#') {
			startIndex = 1;
		}

		unsigned int intValue;
		std::istringstream(std::string(hexColor.substr(startIndex))) >> std::hex >> intValue;

		float red = static_cast<float>((intValue >> 16) & 0xFF) / 255.0f;
		float green = static_cast<float>((intValue >> 8) & 0xFF) / 255.0f;
		float blue = static_cast<float>(intValue & 0xFF) / 255.0f;
		float alpha = 1.0f;

		// Jika ada 8 digit (termasuk alpha), atur alpha sesuai
		if (hexColor.length() == startIndex + 8) {
			alpha = static_cast<float>((intValue >> 24) & 0xFF) / 255.0f;
		}

		return T(red, green, blue, alpha);
	}

}
