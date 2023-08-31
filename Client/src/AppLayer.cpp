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

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			std::cout << m_ProjectionMatrix[row][col] << ' ';
		}
		std::cout << '\n';
	}

	m_SquareColor = { 1.0f, 0.8f, 0.3f, 1.0f };

	m_ShaderSquare = Karem::CreateShader("res\\shader\\position_color_vertex.glsl", "res\\shader\\position_color_fragment.glsl");
	uint32_t shaderID = m_ShaderSquare->GetShaderID();
	glUseProgram(shaderID);

	//m_ShaderSquare->Bind();
	m_ShaderSquare->UpdateUniform("uProjection", (void*)glm::value_ptr(m_ProjectionMatrix));
	m_ShaderSquare->UpdateUniform("uColor", (void*)glm::value_ptr(m_SquareColor));

	m_VertexArraySquare = Karem::CreateVertexArray();
	std::shared_ptr<Karem::VertexBuffer> squareVertexBuffer = Karem::CreateVertexBuffer((void*)squareVertices, sizeof(squareVertices));
	std::shared_ptr<Karem::IndexBuffer> squareIndexBuffer = Karem::CreateIndexBuffer((void*)squareIndices, 6);
	m_VertexArraySquare->AddVertexBuffer(squareVertexBuffer);
	m_VertexArraySquare->SetIndexBuffer(squareIndexBuffer);

	{
		Karem::BufferLayout layout = {
			{ Karem::ShaderDataType::Vec3, "aPos" },
		};
		squareVertexBuffer->SetLayout(layout);
	}

	squareVertexBuffer->ApplyLayout();
	m_VertexArraySquare->UnBind();
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

	Karem::Renderer::Draw(m_VertexArraySquare, m_ShaderSquare);
}