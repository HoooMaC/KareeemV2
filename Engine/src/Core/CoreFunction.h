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

	glm::vec4 HexToVec4(std::string_view hexColor);

}
