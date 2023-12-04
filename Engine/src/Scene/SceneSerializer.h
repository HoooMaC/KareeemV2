#pragma once
#include "Scene/Scene.h"

#include <memory>
#include <string_view>

namespace Karem {

	class SceneSerializer
	{
	public:
		static void Serialize(const std::shared_ptr<Scene>& scene, std::string_view filepath);
		static bool Deserialize(const std::shared_ptr<Scene>& scene, std::string_view filepath);
	};

}