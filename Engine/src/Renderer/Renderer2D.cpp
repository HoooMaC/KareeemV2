#include "Core/Kareeem.h"
#include "Renderer2D.h"

#include "Core/CoreFunction.h"

#include "Renderer/Material.h"
#include "Renderer/BaseShader.h"
#include "Renderer/BaseBuffer.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/RenderCommand.h"

#include <glm/glm.hpp>

namespace Karem {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
	};

	struct BufferData
	{
		static constexpr uint32_t maxVertex = 1000;
		static constexpr uint32_t maxIndexBuffer = maxVertex * 6;

		std::array<Vertex, maxVertex> vertexBuffer;
		std::array<uint32_t, maxIndexBuffer> indexBuffer;
		static uint32_t vertexIndex;
		static uint32_t indicesIndex;
	};

	uint32_t BufferData::vertexIndex = 0;
	uint32_t BufferData::indicesIndex = 0;

	struct Renderer2DData
	{
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Material> m_Material;
	};

	static Renderer2DData* s_Data;
	static BufferData* s_SceneBuffer;
	void Renderer2D::Initialize()
	{
		s_Data = new Renderer2DData;
		s_Data->m_Shader = CreateShader("res\\shader\\position_color_vertex.glsl", "res\\shader\\position_color_fragment.glsl");
		s_Data->m_VertexArray = CreateVertexArray();

		std::shared_ptr<VertexBuffer> vertexBuffer = CreateVertexBuffer(nullptr, BufferData::maxVertex);
		std::shared_ptr<IndexBuffer> indexBuffer = CreateIndexBuffer(nullptr, BufferData::maxIndexBuffer);

		// We need a vertex and index buffer
		BufferLayout layout = s_Data->m_Shader->GetShaderAttributes();
		s_Data->m_Shader->Bind();
		s_Data->m_VertexArray->ApplyShaderLayout(layout);

		s_Data->m_Material = CreateMaterial();
		UniformCache uniforms = s_Data->m_Shader->GetShaderUniforms();
		s_Data->m_Material->SetUniformCache(uniforms);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene()
	{
	}

	void Renderer2D::EndScene()
	{
		//RendererCommand::Draw(s_Data->m_Shader->GetCount);
	}

	void Renderer2D::SubmitQuad(const std::string& objId, const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
	{
		GenerateQuad(pos, size, color);
	}

	void Renderer2D::GenerateQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
	{
		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::array<Vertex, 4> newQuad =
		{
			Vertex{ glm::vec3(pos.x - halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(0.0f, 0.0f), 0.0f }, // kiri bawah
			Vertex{ glm::vec3(pos.x + halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(1.0f, 0.0f), 0.0f }, // kanan bawah
			Vertex{ glm::vec3(pos.x + halfWidth, pos.y + halfHeight, pos.z), color, glm::vec2(1.0f, 1.0f), 0.0f }, // kanan atas
			Vertex{ glm::vec3(pos.x - halfWidth, pos.y + halfHeight, pos.z), color, glm::vec2(0.0f, 1.0f), 0.0f }  // kiri atas
		};

		std::array<uint32_t, 6> newQuadIndices = 
		{ 
			BufferData::indicesIndex,
			BufferData::indicesIndex + 1,
			BufferData::indicesIndex + 2,
			BufferData::indicesIndex + 2,
			BufferData::indicesIndex + 3,
			BufferData::indicesIndex 
		};

		if (BufferData::vertexIndex + newQuad.size() <= BufferData::maxVertex)
		{
			std::copy(newQuad.begin(), newQuad.end(), s_SceneBuffer->vertexBuffer.begin() + BufferData::vertexIndex);

			BufferData::vertexIndex += (uint32_t)newQuad.size();
		}

		if (BufferData::indicesIndex + newQuadIndices.size() <= BufferData::maxIndexBuffer)
		{
			std::copy(newQuadIndices.begin(), newQuadIndices.end(), s_SceneBuffer->indexBuffer.begin() + BufferData::vertexIndex);

			BufferData::vertexIndex += (uint32_t)newQuadIndices.size();
		}

	}

	void Renderer2D::Validate()
	{
	}

	void Renderer2D::Bind()
	{
	}

	void Renderer2D::Flush()
	{
		//s_SceneBuffer->vertexBuffer.
	}

}