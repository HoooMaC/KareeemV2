#include "AppLayer.h"


#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void AppLayer::OnAttach()
{
	float squareVertices[4 * 3] =
	{
		   0.0f,  0.0f, 0.6f, // kiri bawah
		   1.5f,  0.0f, 0.6f, // kanan bawah
		   1.5f,  1.5f, 0.6f, // kanan atas
		   0.0f,  1.5f, 0.6f  // kiri atas
	};
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	m_ProjectionMatrix = glm::ortho(0.0f, 16.0f, 0.0f, 9.0f);

	m_SquareColor = { 1.0f, 0.8f, 0.3f, 1.0f };

	std::shared_ptr<Karem::VertexBuffer> squareVertexBuffer = Karem::CreateVertexBuffer((void*)squareVertices, sizeof(squareVertices));
	std::shared_ptr<Karem::IndexBuffer> squareIndexBuffer = Karem::CreateIndexBuffer((void*)squareIndices, 6);

	Karem::Renderer::AddVertexBuffer(squareVertexBuffer);
	Karem::Renderer::SetIndexBuffer(squareIndexBuffer);
	
	//maybe this can be automated by glGetActiveAttrib
	Karem::BufferLayout layout = {
		{ Karem::ShaderDataType::Vec3, "aPos" },
	};
	squareVertexBuffer->SetLayout(layout);

	squareVertexBuffer->ApplyLayout();
}

void AppLayer::OnUpdate()
{
	static bool increment = true;
	if (increment)
	{
		m_SquareColor.g += 0.01f;
	}
	else
	{
		m_SquareColor.g -= 0.01f;
	}
	if (m_SquareColor.g < 0.0f or m_SquareColor.g > 1.0f)
	{
		increment = !increment;
	}

	Karem::Renderer::UpdateUniform("uColor", (void*)glm::value_ptr(m_SquareColor));
}