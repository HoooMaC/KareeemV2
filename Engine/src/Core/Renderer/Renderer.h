#pragma once

#include <memory>

#include "RenderCommand.h"

#include "Core/Renderer/BaseBuffer.h"
#include "Core/Renderer/BaseShader.h"
#include "Core/Renderer/BaseRendererAPI.h"
#include "Core/Renderer/OrthographicCamera.h"


namespace Karem {

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Draw();

		static void SetShader(const std::shared_ptr<Shader>& shader);
		static void SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray);

		static void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		static void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		static void UpdateUniform(const std::string& name, void* data);

		inline static API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		static std::shared_ptr<Shader> m_RendererShader;
		static std::shared_ptr<VertexArray> m_RendererVertexArray;
	};

}
