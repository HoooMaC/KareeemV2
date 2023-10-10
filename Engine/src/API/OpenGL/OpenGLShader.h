#pragma once

#include <unordered_map>
#include <string_view>

#include "Renderer/BaseShader.h"
#include "Renderer/Uniform.h"

namespace Karem{


	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string_view vertexShader, std::string_view fragmentShader);
		~OpenGLShader();


		void Bind() const override;
		void UnBind() const override;

		BufferLayout GetShaderAttributes() const override;
		UniformCache GetShaderUniforms() const override;
	private:
		void Clear();

		std::string ReadFile(std::string_view fileSource);
		void CreateShader(uint32_t vertexShader, uint32_t fragmentShader);
		uint32_t CompileShader(std::string_view source, uint32_t shaderType);

	private:
		uint32_t m_RendererID;
	};

}
