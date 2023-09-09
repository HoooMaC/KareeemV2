#pragma once

#include <unordered_map>

#include "Renderer/BaseShader.h"
#include "Renderer/Uniform.h"

namespace Karem{


	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexShader, const std::string& fragmentShader);
		~OpenGLShader();


		void Bind() const override;
		void UnBind() const override;

		uint32_t GetID() const override { return m_RendererID; }
	private:
		void Clear();

		std::string ReadFile(const std::string& fileSource);
		void CreateShader(uint32_t vertexShader, uint32_t fragmentShader);
		uint32_t CompileShader(const std::string& source, uint32_t shaderType);

	private:
		uint32_t m_RendererID;
	};

}
