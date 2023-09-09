#pragma once

#include "Renderer/BaseShader.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/BaseRendererAPI.h"

namespace Karem {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;

		void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void ClearColor(const glm::vec4& color) override;
		void ClearColor(const std::string& hexColor) override;
		void Clear() override;

		void Draw(uint32_t count) override;

		BufferLayout GetShaderAttributes(const std::shared_ptr<Shader>& shader);
	};

}
