#include "Core/Kareeem.h"
#include "Uniform.h"

// TEMPORARY . Because uniform sholud be general for any API
#include <glad/glad.h>

namespace Karem {

	void UniformCache::SetUniformData(const std::string& name, void* data)
	{
		auto uniformIterator = m_UnifomList.find(name);
		if (uniformIterator == m_UnifomList.end()) {
			uniformIterator = m_UnifomList.find(name + "[0]");
			if (uniformIterator == m_UnifomList.end()) {
				ENGINE_WARN("Uniform {} not found.", name);
				return;
			}
		}

		uniformIterator->second.Data = data;
	}

	// WARNING : Consider the lifetime of the object
	void Karem::UniformCache::UploadUniformData(const UniformElement& uniform)
	{
		switch (uniform.Type)
		{
		case GL_FLOAT:			return glUniform1fv(uniform.Location, uniform.Count, (const float*)uniform.Data);
		case GL_FLOAT_VEC2:		return glUniform2fv(uniform.Location, uniform.Count, (const float*)uniform.Data);
		case GL_FLOAT_VEC3:		return glUniform3fv(uniform.Location, uniform.Count, (const float*)uniform.Data);
		case GL_FLOAT_VEC4:		return glUniform4fv(uniform.Location, uniform.Count, (const float*)uniform.Data);
		case GL_FLOAT_MAT2:		return glUniformMatrix2fv(uniform.Location, uniform.Count, GL_FALSE, (const float*)uniform.Data);
		case GL_FLOAT_MAT3:		return glUniformMatrix3fv(uniform.Location, uniform.Count, GL_FALSE, (const float*)uniform.Data);
		case GL_FLOAT_MAT4:		return glUniformMatrix4fv(uniform.Location, uniform.Count, GL_FALSE, (const float*)uniform.Data);
		case GL_INT:			return glUniform1iv(uniform.Location, uniform.Count, (const int*)uniform.Data);
		case GL_INT_VEC2:		return glUniform2iv(uniform.Location, uniform.Count, (const int*)uniform.Data);
		case GL_INT_VEC3:		return glUniform3iv(uniform.Location, uniform.Count, (const int*)uniform.Data);
		case GL_SAMPLER_2D:		return glUniform1iv(uniform.Location, uniform.Count, (const int*)uniform.Data);
		case GL_BOOL:			ENGINE_INFO("BOOLEAN HASN'T SUPPORTED YET"); break;
		default:
			ENGINE_ASSERT(false, "INVALID UNIFORM TYPE");
			return;
		}
	}

	void UniformCache::Validate()
	{
		for (auto [name, uniform] : m_UnifomList)
		{
			if (uniform.Data == nullptr)
			{
				ENGINE_ASSERT(false, "Unitilialized Uniform {}", name);
			}
			UploadUniformData(uniform);
		}
	}

}