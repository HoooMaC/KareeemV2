#pragma once

#include "Core/Renderer/BaseShader.h"

namespace Karem{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexShader, const std::string& fragmentShader);
		~OpenGLShader();

		void Init(const std::string& vertexShader, const std::string& fragmentShader) override;
		void Clear() override;

		void Bind() const override;
		void UnBind() const override;

	private:
		uint32_t m_RendererID;
	};

}
