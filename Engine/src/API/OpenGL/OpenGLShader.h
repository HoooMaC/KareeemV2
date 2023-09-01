#pragma once

#include <unordered_map>

#include "Core/Renderer/BaseShader.h"

namespace Karem{

	struct UniformAttrib
	{
		uint32_t Type;
		uint32_t Count;
		int32_t Location;
		void* Data;

		UniformAttrib() = default;
		UniformAttrib(uint32_t type, uint32_t count, int32_t location)
			: Type(type), Count(count), Location(location), Data(nullptr) {}
	};

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexShader, const std::string& fragmentShader);
		~OpenGLShader();

		void Clear() override;

		void Bind() const override;
		void UnBind() const override;

		void BindAndUploadUniform()	const override;
		void UpdateUniform(const std::string& name, void* data) override;
	private:
		
		std::string ReadFile(const std::string& fileSource);
		void CreateShader(uint32_t vertexShader, uint32_t fragmentShader);
		uint32_t CompileShader(const std::string& source, uint32_t shaderType);
		void GetUniform();
		void UploadUniform(const UniformAttrib& uniform) const;

	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, UniformAttrib> m_UniformContainer;
	};

}
