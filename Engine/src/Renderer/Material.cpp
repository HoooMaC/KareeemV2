#include "Core/Kareeem.h"
#include "Material.h"

namespace Karem {

	Material::Material(const std::shared_ptr<Shader>& shader)
	{
	}

	std::shared_ptr<Material> CreateMaterial()
	{
		return std::make_shared<Material>();
	}

}