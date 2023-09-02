#pragma once

#include <glm/glm.hpp>

#include "Renderer/BaseShader.h"
#include "Renderer/BaseBuffer.h"

namespace Karem {

	enum class API
	{
		None = 0, OpenGL
	};

	class RendererAPI
	{
	public:
		virtual void Init() = 0;

		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void ClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void Draw(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_Api; }
	private:
		static API s_Api;
	};
}
