#include "Core/Kareeem.h"
#include "Core/CoreFunction.h"

#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Karem {

	void OpenGLRendererAPI::Init()
	{
	}

	void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::ClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::ClearColor(const std::string& hexColor)
	{
		glm::vec4 color = HexToVec4(hexColor);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::Draw(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	BufferLayout OpenGLRendererAPI::GetShaderAttributes(const std::shared_ptr<Shader>& shader)
	{
		std::vector<BufferElement> elements;

		int32_t attributeNumber;
		int32_t maxBuffer;
		glGetProgramiv(shader->GetID(), GL_ACTIVE_ATTRIBUTES, &attributeNumber);
		glGetProgramiv(shader->GetID(), GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxBuffer);

		//ENGINE_DEBUG("Attribute number {}", attributeNumber);
		//ENGINE_DEBUG("Attribute max number {}", maxBuffer);

		for (int i = 0; i < attributeNumber; i++)
		{
			int32_t attributeCount;
			uint32_t attributeType;
			int32_t attributeLength;
			int32_t attributeIndex = i;
			char* attributeName = new char[maxBuffer];

			glGetActiveAttrib(shader->GetID(), i, maxBuffer, &attributeLength, &attributeCount, &attributeType, attributeName);

			//ENGINE_DEBUG("=================");
			//ENGINE_DEBUG("{} Uniform name : {}", i, uniformName);
			//ENGINE_DEBUG("Uniform Type : {}", uniformType);
			//ENGINE_DEBUG("Uniform Count : {}", uniformCount);
			//ENGINE_DEBUG("Uniform Length : {}", uniformLength);
			//ENGINE_DEBUG("Uniform Location : {}", uniformLocation);
			//ENGINE_DEBUG("=================");

			ShaderDataType type = UintToShaderDataType(attributeType);
			BufferElement element(type, attributeName);
			elements.push_back(element);
		}

		return elements;
	}

}