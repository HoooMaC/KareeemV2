#pragma once

#include <memory>

#include "RenderCommand.h"

#include "Renderer/BaseBuffer.h"
#include "Renderer/BaseShader.h"
#include "Renderer/BaseRendererAPI.h"
#include "Renderer/OrthographicCamera.h"


namespace Karem {

	class Renderer
	{
	public:
		static void Init();

		//static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Draw();

		static void SetShader(const std::shared_ptr<Shader>& shader);
		static void SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray);

		static void UpdateUniform(const std::string& name, void* data);

		inline static API GetAPI() { return RendererAPI::GetAPI(); }
	};

}
