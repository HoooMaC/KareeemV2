#include "TriangleLayer.h"

void TriangleLayer::OnAttach()
{
	float middleX = 0, middleY = 0;
	float size = 0.5;

	float triangleVertices[] =
	{
		   middleX - (size / 2),  middleY - (size / 2), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // kiri bawah
		   middleX + (size / 2),  middleY - (size / 2), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // kanan bawah
		   middleX			   ,  middleY + (size / 2), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f // atas
	};
	uint32_t triagleIndices[6] = { 0, 1, 2 };

	m_TriangleVertexArray = Karem::CreateVertexArray();
	m_TriangleShader = Karem::CreateShader("res\\shader\\triangle_Vertex_Shader.glsl", "res\\shader\\triangle_Fragment_Shader.glsl");


	// mungkin disini harusnya nggak ada, apalagi kalau batch rendering
	std::shared_ptr<Karem::VertexBuffer> triagleVertexBuffer = Karem::CreateVertexBuffer((void*)triangleVertices, sizeof(triangleVertices));
	std::shared_ptr<Karem::IndexBuffer> triagleIndexBuffer = Karem::CreateIndexBuffer((void*)triagleIndices, 6);

	Karem::Renderer::SetVertexArray(m_TriangleVertexArray);
	Karem::Renderer::SetShader(m_TriangleShader);

	Karem::Renderer::AddVertexBuffer(triagleVertexBuffer);
	Karem::Renderer::SetIndexBuffer(triagleIndexBuffer);

	//maybe this can be automated by glGetActiveAttrib
	Karem::BufferLayout layout = {
		{ Karem::ShaderDataType::Vec3, "aPos" },
		{ Karem::ShaderDataType::Vec4, "aColor" }
	};

	triagleVertexBuffer->SetLayout(layout);

	triagleVertexBuffer->ApplyLayout();
}

void TriangleLayer::OnUpdate(Karem::TimeStep ts)
{
	Karem::Renderer::SetShader(m_TriangleShader);
	Karem::Renderer::SetVertexArray(m_TriangleVertexArray);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3({10.0f, 10.0f, 0.0f}));

	Karem::Renderer::UpdateUniform("uProjectionView", (void*)glm::value_ptr(m_ProjectionViewMatrix));
	Karem::Renderer::UpdateUniform("uModel", (void*)glm::value_ptr(transform));

	Karem::Renderer::Draw();
}
