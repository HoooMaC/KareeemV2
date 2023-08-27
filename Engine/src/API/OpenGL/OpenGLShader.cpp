#include "Core/Kareeem.h"

#include "OpenGLShader.h"

#include <glad/glad.h>

namespace Karem {

	OpenGLShader::OpenGLShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		std::string vertexShader = ReadFile(vertexShaderFilePath);
		std::string fragmentShader = ReadFile(fragmentShaderFilePath);

		// TO DO : separate the compilation of each shader
		// and check the uniform 

		const char* vertexShaderSource = vertexShader.c_str();
		const char* fragmentShaderSource = fragmentShader.c_str();

		GLuint vertexShaderId = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
		GLuint fragmentShaderId = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

		CreateShader(vertexShaderId, fragmentShaderId);

		GetUniform();
	}

	void OpenGLShader::GetUniform()
	{
		int32_t uniformNum;
		int32_t maxBuffer;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &uniformNum);
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxBuffer);

		for (int i = 0; i < uniformNum; i++)
		{
			int32_t uniformCount;
			uint32_t uniformType;
			int32_t uniformLength;
			int32_t uniformLocation;
			char* uniformName = new char[maxBuffer];
			glGetActiveUniform(m_RendererID, i, maxBuffer, &uniformLength, &uniformCount, &uniformType, uniformName);
			uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

			ENGINE_DEBUG("Uniform name : {}", uniformName);
			ENGINE_DEBUG("Uniform Type : {}", uniformType);
			ENGINE_DEBUG("Uniform Count : {}", uniformCount);
			ENGINE_DEBUG("Uniform Length : {}", uniformLength);
			ENGINE_DEBUG("Uniform Location : {}", uniformLocation);

			UniformAttrib uniform(uniformType, uniformCount, uniformLocation);

			m_UniformContainer[uniformName] = uniform;
		}

	}

	void OpenGLShader::UpdateUniform(const std::string& name, void* data)
	{
		// TO DO : Perlu pengecekan apakah uniform dengan nama "name" ada atau 
		m_UniformContainer[name].Data = data;
	}

	void OpenGLShader::UploadUniform(const UniformAttrib& uniform) const
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
		case GL_BOOL:			ENGINE_INFO("BOOLEAN HASN'T SUPPORTED YET");
		default:
			ENGINE_WARN("INVALID UNIFORM TYPE");
		}
	}

	void OpenGLShader::CreateShader(uint32_t vertexShader, uint32_t fragmentShader)
	{
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);

		// TO DO : need to check the link status
		int success;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(m_RendererID, 512, nullptr, infoLog);
			ENGINE_ERROR("Error linking shader program:");
			glDeleteProgram(m_RendererID);
			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	uint32_t OpenGLShader::CompileShader(const std::string& source, uint32_t type)
	{
		uint32_t programId = glCreateShader(type);
		const char* shaderSource = source.c_str();
		glShaderSource(programId, 1, &shaderSource, nullptr);
		glCompileShader(programId);
		return programId;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::BindAndUploadUniform() const
	{
		for (const auto& [name, uniform] : m_UniformContainer)
		{
			UploadUniform(uniform);
		}
	}

	std::string OpenGLShader::ReadFile(const std::string& fileSource)
	{
		// TODO: insert return statement here
		std::ifstream input(fileSource);

		if (!input.is_open())
		{
			// TODO : make some Assertion here
			ENGINE_ERROR("FILE IS NOT FOUND");
			__debugbreak();
			return "";
		}

		std::stringstream buffer;
		buffer << input.rdbuf();

		//ENGINE_DEBUG("{}", buffer.str());

		input.close();

		return buffer.str();
	}

	OpenGLShader::~OpenGLShader()
	{
		Clear();
	}

	void OpenGLShader::Clear()
	{
		glDeleteProgram(m_RendererID);
	}
}