#include "Core/Kareeem.h"

#include "OpenGLShader.h"

#include <glad/glad.h>

namespace Karem {

	OpenGLShader::OpenGLShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		std::string vertexShader = ReadFile(vertexShaderFilePath);
		std::string fragmentShader = ReadFile(fragmentShaderFilePath);
		Init(vertexShader, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		Clear();
	}

	void OpenGLShader::Init(const std::string& vertexShader, const std::string& fragmentShader)
	{
		const char* vertexShaderSource = vertexShader.c_str();
		// Compile dan link shader
		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShaderId);
		// TO DO : need to check the compile status

		const char* fragmentShaderSource = fragmentShader.c_str();
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShaderId);
		// TO DO : need to check the compile status

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShaderId);
		glAttachShader(m_RendererID, fragmentShaderId);
		glLinkProgram(m_RendererID);
		// TO DO : need to check the link status

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

	}

	void OpenGLShader::Clear()
	{
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
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

}