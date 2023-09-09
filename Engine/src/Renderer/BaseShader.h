#pragma once

#include <memory>
#include <string>

namespace Karem {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetID() const = 0;
	};

	std::shared_ptr<Shader> CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

}
