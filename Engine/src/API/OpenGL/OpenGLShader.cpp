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

		//ENGINE_DEBUG("Uniform number {}", uniformNum);
		//ENGINE_DEBUG("Uniform max number {}", maxBuffer);

		for (int i = 0; i < uniformNum; i++)
		{
			int32_t uniformCount;
			uint32_t uniformType;
			int32_t uniformLength;
			int32_t uniformLocation;
			char* uniformName = new char[maxBuffer];

			glGetActiveUniform(m_RendererID, i, maxBuffer, &uniformLength, &uniformCount, &uniformType, uniformName);
			uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

			//ENGINE_DEBUG("=================");
			//ENGINE_DEBUG("{} Uniform name : {}", i, uniformName);
			//ENGINE_DEBUG("Uniform Type : {}", uniformType);
			//ENGINE_DEBUG("Uniform Count : {}", uniformCount);
			//ENGINE_DEBUG("Uniform Length : {}", uniformLength);
			//ENGINE_DEBUG("Uniform Location : {}", uniformLocation);
			//ENGINE_DEBUG("=================");

			UniformAttrib uniform(uniformType, uniformCount, uniformLocation);

			m_UniformContainer[uniformName] = uniform;
		}

	}

	void OpenGLShader::UpdateUniform(const std::string& name, void* data)
	{
		// TODO: Check if the uniform with the given "name" exists
		auto uniformIterator = m_UniformContainer.find(name);
		if (uniformIterator == m_UniformContainer.end()) {
			ENGINE_WARN("Uniform {} not found.", name);
			return;
		}

		uniformIterator->second.Data = data;
		UploadUniform(uniformIterator->second);
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
			return;
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
		uint32_t shaderId = glCreateShader(type);
		const char* shaderSource = source.c_str();
		glShaderSource(shaderId, 1, &shaderSource, nullptr);
		glCompileShader(shaderId);

		// TODO: Check the compilation status
		int success;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
			ENGINE_ERROR("Error compiling shader :\n{}", infoLog);
			glDeleteShader(shaderId);
			return 0; // or appropriate error value
		}

		return shaderId;
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
		glUseProgram(m_RendererID);
		for (const auto& [name, uniform] : m_UniformContainer)
		{
			//ENGINE_TRACE("{}", name);
			if (uniform.Data == nullptr)
			{
				// this should be changed with ASSERTION
				ENGINE_WARN("Unitiliazed Uniform : {}", name);
				__debugbreak();
			}


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