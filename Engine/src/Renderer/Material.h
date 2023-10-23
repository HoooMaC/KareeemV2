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

		// Be aware to use this data, make sure the void* data is not NULL
		void SetUniformData(const std::string& name, void* data)
		{
			ENGINE_ASSERT(data, "ERROR!!! YOUR DATA IS NULL");
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


}
