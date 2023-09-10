#include "Core/Kareeem.h"

#include "OpenGLShader.h"

#include <glad/glad.h>

#include "Renderer/BufferLayout.h"

namespace Karem {

	OpenGLShader::OpenGLShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		std::string vertexShader = ReadFile(vertexShaderFilePath);
		std::string fragmentShader = ReadFile(fragmentShaderFilePath);

		const char* vertexShaderSource = vertexShader.c_str();
		const char* fragmentShaderSource = fragmentShader.c_str();

		GLuint vertexShaderId = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
		GLuint fragmentShaderId = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

		CreateShader(vertexShaderId, fragmentShaderId);
		
		ENGINE_DEBUG("Creating a shader with id [{}]", m_RendererID);
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
		ENGINE_DEBUG("Binding shader with id [{}]", m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	BufferLayout OpenGLShader::GetShaderAttributes() const
	{
		std::vector<BufferElement> elements;

		int32_t attributeNumber;
		int32_t maxBuffer;
		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTES, &attributeNumber);
		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxBuffer);

		//ENGINE_DEBUG("Attribute number {}", attributeNumber);
		//ENGINE_DEBUG("Attribute max number {}", maxBuffer);

		for (int i = 0; i < attributeNumber; i++)
		{
			int32_t attributeCount;
			uint32_t attributeType;
			int32_t attributeLength;
			int32_t attributeIndex = i;
			char* attributeName = new char[maxBuffer];

			glGetActiveAttrib(m_RendererID, i, maxBuffer, &attributeLength, &attributeCount, &attributeType, attributeName);

			//ENGINE_DEBUG("=================");
			//ENGINE_DEBUG("{} Uniform name : {}", i, attributeName);
			//ENGINE_DEBUG("Attribute Type : {}", attributeType);
			//ENGINE_DEBUG("Attribute Count : {}", attributeCount);
			//ENGINE_DEBUG("Attribute Length : {}", attributeLength);
			//ENGINE_DEBUG("Attribute Location : {}", attributeLocation);
			//ENGINE_DEBUG("=================");

			ShaderDataType type = UintToShaderDataType(attributeType);
			BufferElement element(type, attributeName);
			elements.push_back(element);
		}

		return elements;
	}

	UniformCache OpenGLShader::GetShaderUniforms() const
	{
		std::unordered_map<std::string, UniformElement> uniformList;

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

			UniformElement uniform(uniformType, uniformCount, uniformLocation);

			uniformList[uniformName] = uniform;
		}

		return uniformList;
	}

	std::string OpenGLShader::ReadFile(const std::string& fileSource)
	{
		std::ifstream input(fileSource);

		if (!input.is_open())
		{
			// TODO : make some Assertion here
			ENGINE_ASSERT(false, "FILE IS NOT FOUND");
			return "";
		}

		std::stringstream buffer;
		buffer << input.rdbuf();

		//ENGINE_DEBUG("\n{}", buffer.str());

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