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

		void SetUniformData(std::string_view name, void* data)
		{
			m_UniformList.SetUniformData(name, data);
		}

		void ValidateMaterial()
		{
			m_UniformList.Validate();
			//we should also validate other data 
		}
	private:
		UniformCache m_UniformList;
	};

	std::shared_ptr<Material> CreateMaterial();

}
