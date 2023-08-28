#include "Core/Kareeem.h"

#include "Renderer.h"

// I think this shouldn't be here
#include "API/OpenGL/OpenGLShader.h"

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
		std::dynamic_pointer_cast<Karem::OpenGLShader>(shader)->BindAndUploadUniform();

		RendererCommand::Draw(vertexArray);
	}

}