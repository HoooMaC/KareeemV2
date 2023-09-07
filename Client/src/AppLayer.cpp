	#include "AppLayer.h"


#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void AppLayer::OnAttach()
{
	float middleX = 0, middleY = 0;
	float size = 10;

	glm::vec4 color = Karem::HexToVec4("#0099DD");

	float squareVertices[4 * 9] =
	{
		   middleX - (size / 2),  middleY - (size / 2), 0.0f, color.r, color.g, color.b, color.a, 0.0f, 0.0f, // kiri bawah
		   middleX + (size / 2),  middleY - (size / 2), 0.0f, color.r, color.g, color.b, color.a, 1.0f, 0.0f, // kanan bawah
		   middleX + (size / 2),  middleY + (size / 2), 0.0f, color.r, color.g, color.b, color.a, 1.0f, 1.0f, // kanan atas
		   middleX - (size / 2),  middleY + (size / 2), 0.0f, color.r, color.g, color.b, color.a, 0.0f, 1.0f  // kiri atas
	};

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	m_SquareShader = Karem::CreateShader("res\\shader\\position_color_vertex.glsl", "res\\shader\\position_color_fragment.glsl");
	m_SquareVertexArray = Karem::CreateVertexArray();
	m_TextureBasic = Karem::CreateTexture2D("res\\texture\\magenta_glazed_terracotta.png");

	Karem::Renderer::SetShader(m_SquareShader);
	Karem::Renderer::SetVertexArray(m_SquareVertexArray);
	m_TextureSlot = m_TextureBasic->GetSlot();
	Karem::Renderer::UpdateUniform("uTexture", (void*)&m_TextureSlot);

	// mungkin disini harusnya nggak ada, apalagi kalau batch rendering
	std::shared_ptr<Karem::VertexBuffer> squareVertexBuffer = Karem::CreateVertexBuffer((void*)squareVertices, sizeof(squareVertices));
	std::shared_ptr<Karem::IndexBuffer> squareIndexBuffer = Karem::CreateIndexBuffer((void*)squareIndices, 6);

	Karem::Renderer::AddVertexBuffer(squareVertexBuffer);
	Karem::Renderer::SetIndexBuffer(squareIndexBuffer);
	
	//maybe this can be automated by glGetActiveAttrib
	Karem::BufferLayout layout = {
		{ Karem::ShaderDataType::Vec3, "aPos" },
		{ Karem::ShaderDataType::Vec4, "aColor" },
		{ Karem::ShaderDataType::Vec2, "aTexCoord" }
	};
	squareVertexBuffer->SetLayout(layout);

	squareVertexBuffer->ApplyLayout();
}

void AppLayer::OnUpdate(Karem::TimeStep ts)
{
	static glm::vec3 pos(0.0f);

	if (Karem::Input::IsKeyPressed(KAREM_KEY_L))
	{
		pos.x += 10.0f * ts;
	}
	else if (Karem::Input::IsKeyPressed(KAREM_KEY_J))
	{
		pos.x -= 10.0f * ts;
	}

	if (Karem::Input::IsKeyPressed(KAREM_KEY_I))
	{
		pos.y += 10.0f * ts;
	}
	else if (Karem::Input::IsKeyPressed(KAREM_KEY_K))
	{
		pos.y -= 10.0f * ts;
	}

	Karem::Renderer::SetShader(m_SquareShader);
	Karem::Renderer::SetVertexArray(m_SquareVertexArray);

/*	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
	for(int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { (float)x + pos.x, float(y) + pos.y, 0.0f}) * scale;
			Karem::Renderer::UpdateUniform("uProjectionView", (void*)glm::value_ptr(m_ProjectionViewMatrix));
			Karem::Renderer::UpdateUniform("uModel", glm::value_ptr(transform));
			Karem::Renderer::Draw();
		}
	}
*/
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f }) * scale;
	Karem::Renderer::UpdateUniform("uProjectionView", (void*)glm::value_ptr(m_ProjectionViewMatrix));
	Karem::Renderer::UpdateUniform("uModel", glm::value_ptr(transform));
	Karem::Renderer::Draw();

	// disini seharusnya Submit data

}