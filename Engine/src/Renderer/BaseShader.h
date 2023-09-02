#pragma once

#include <memory>
#include <string>

namespace Karem {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Clear() = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void BindAndUploadUniform() const = 0;
		virtual void UpdateUniform(const std::string& name, void* data) = 0;
	};

	std::shared_ptr<Shader> CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

}
