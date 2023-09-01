#include "AppLayer.h"


#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void AppLayer::OnAttach()
{

	float middleX = 0, middleY = 0;
	float size = 6;

	float squareVertices[4 * 3] =
	{
		   middleX - (size / 2),  middleY - (size / 2), 0.6f, // kiri bawah
		   middleX + (size / 2),  middleY - (size / 2), 0.6f, // kanan bawah
		   middleX + (size / 2),  middleY + (size / 2), 0.6f, // kanan atas
		   middleX - (size / 2),  middleY + (size / 2), 0.6f  // kiri atas
	};
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	// mungkin disini harusnya nggak ada, apalagi kalau batch rendering
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

void AppLayer::OnUpdate(Karem::TimeStep ts)
{
	// disini seharusnya Submit data
}