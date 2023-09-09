#pragma once

#include "Renderer/BaseShader.h"
#include "Renderer/Uniform.h"

#include <memory>
#include <vector>

namespace Karem {

	class Material
	{
	public:
		Material(const std::shared_ptr<Shader>& shader);
	private:
		std::vector<Uniform> m_UniformCache;
	};

}
