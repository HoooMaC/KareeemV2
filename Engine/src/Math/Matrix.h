#pragma once

#include <glm/glm.hpp>

namespace Karem::Math {

	bool DecomposeMatrix(const glm::mat4& source, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

}