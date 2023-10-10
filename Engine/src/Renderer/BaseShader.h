#pragma once

#include <memory>
#include <string>

namespace Karem {

	class BufferLayout;
	class UniformCache;

	enum class ShaderDataType
	{
		None = 0, Float, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual BufferLayout GetShaderAttributes() const = 0;
		virtual UniformCache GetShaderUniforms() const = 0;
	};

	std::shared_ptr<Shader> CreateShader(std::string_view vertexShader, std::string_view fragmentShader);

}
