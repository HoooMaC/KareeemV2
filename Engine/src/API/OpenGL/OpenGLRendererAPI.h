#pragma once

#include "Renderer/BaseRendererAPI.h"

namespace Karem {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;

		void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void ClearColor(const glm::vec4& color) override;
		void Clear() override;

		void Draw(const std::shared_ptr<VertexArray>& vertexArray) override;
	};

}
