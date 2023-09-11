#include "Core/Kareeem.h"
#include "Uniform.h"

// this is temporary. Because uniform sholud be general for any API
#include <glad/glad.h>

namespace Karem {

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

}