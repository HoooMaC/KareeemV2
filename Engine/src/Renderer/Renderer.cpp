#include "Core/Kareeem.h"

#include "Renderer.h"


// I think this shouldn't be here
#include "API/OpenGL/OpenGLShader.h"

namespace Karem {

	struct RendererData
	{
		std::shared_ptr<Shader> m_RendererShader;
		std::shared_ptr<VertexArray> m_RendererVertexArray;
	};

	static RendererData s_Data;


	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		//s_Data.m_RendererShader->Bind();
		//s_Data.m_RendererShader->UpdateUniform("uProjectionView", (void*)glm::value_ptr(camera.GetViewProjectionMatrix()));
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Draw()
	{
		//s_Data.m_RendererShader->BindAndUploadUniform();
		//RendererCommand::Draw(s_Data.m_RendererVertexArray);
	}

	void Renderer::SetShader(const std::shared_ptr<Shader>& shader)
	{ 
		s_Data.m_RendererShader = shader; 
	}

	void Renderer::SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray)
	{ 
		s_Data.m_RendererVertexArray = vertexArray;
		s_Data.m_RendererVertexArray->Bind();
	}

	void Renderer::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		s_Data.m_RendererVertexArray->AddVertexBuffer(vertexBuffer);
	}

	void Renderer::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		s_Data.m_RendererVertexArray->SetIndexBuffer(indexBuffer);
	}

	void Renderer::UpdateUniform(const std::string& name, void* data)
	{
		s_Data.m_RendererShader->Bind();
		//s_Data.m_RendererShader->UpdateUniform(name, data);
	}

}