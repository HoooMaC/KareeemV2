#pragma once

#include <memory>

#include "RenderCommand.h"

#include "Core/Renderer/BaseBuffer.h"
#include "Core/Renderer/BaseShader.h"
#include "Core/Renderer/BaseRendererAPI.h"

namespace Karem {

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene();
		static void EndScene();

		static void Draw(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

		inline static API GetAPI() { return RendererAPI::GetAPI(); }
	};

}
