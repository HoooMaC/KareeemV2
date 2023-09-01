#include "Core/Kareeem.h"

#include "Renderer.h"


// I think this shouldn't be here
#include "API/OpenGL/OpenGLShader.h"

namespace Karem {

	std::shared_ptr<Shader> Renderer::m_RendererShader;
	std::shared_ptr<VertexArray> Renderer::m_RendererVertexArray;

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_RendererShader->Bind();
		m_RendererShader->UpdateUniform("uProjection", (void*)glm::value_ptr(camera.GetProjectionMatrix()));
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Draw()
	{
		std::dynamic_pointer_cast<Karem::OpenGLShader>(m_RendererShader)->BindAndUploadUniform();

		RendererCommand::Draw(m_RendererVertexArray);
	}

	void Renderer::SetShader(const std::shared_ptr<Shader>& shader)
	{ 
		m_RendererShader = shader; 
	}

	void Renderer::SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray)
	{ 
		m_RendererVertexArray = vertexArray;
	}

	void Renderer::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		m_RendererVertexArray->AddVertexBuffer(vertexBuffer);
	}

	void Renderer::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_RendererVertexArray->SetIndexBuffer(indexBuffer);
	}

	void Renderer::UpdateUniform(const std::string& name, void* data)
	{
		m_RendererShader->Bind();
		m_RendererShader->UpdateUniform(name, data);
	}

}