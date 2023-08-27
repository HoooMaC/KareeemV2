#include "Core/Kareeem.h"

#include "Renderer.h"

namespace Karem {

	
	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Draw(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();

		RendererCommand::Draw(vertexArray);
	}

}