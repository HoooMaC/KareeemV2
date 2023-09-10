#pragma once

#include "Renderer/BaseShader.h"
#include "Renderer/Uniform.h"

#include <memory>
#include <vector>

namespace Karem {

	class Material
	{
	public:
		Material() = default;
		Material(const std::shared_ptr<Shader>& shader);

		void SetUniformCache(UniformCache& uniforms)
		{
			m_UniformList = uniforms;
		}
	private:
		UniformCache m_UniformList;
	};

	std::shared_ptr<Material> CreateMaterial();

}
