#include "Core/Kareeem.h"
#include "Renderer2D.h"

#include "Core/CoreFunction.h"

#include "Renderer/OrthographicCamera.h"

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
		Vertex()
			: Position(glm::vec3(0.0f)), Color(glm::vec4(0.0f)), TexCoord(glm::vec2(0.0f)), TexIndex(0.0f) {}

		Vertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& texCoord, float texIndex)
			: Position(position), Color(color), TexCoord(texCoord), TexIndex(texIndex) {}
	};

	struct BufferData
	{
		static constexpr uint64_t maxVertex = 12;
		static constexpr uint64_t maxIndexBuffer = maxVertex * 6;

		std::array<Vertex, maxVertex> vertexData;
		std::array<uint32_t, maxIndexBuffer> indicesData;
		static uint32_t vertexIndex;
		static uint32_t indicesIndex;
		static uint32_t indicesOffset;
	};

	struct Renderer2DData
	{
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Material> m_Material;

		// this is temporary
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};

	uint32_t BufferData::vertexIndex = 0;
	uint32_t BufferData::indicesIndex = 0;
	uint32_t BufferData::indicesOffset = 0;


	static Renderer2DData* s_Data;
	static BufferData* s_SceneBuffer;


	void Renderer2D::Initialize()
	{
		s_Data = new Renderer2DData;
		s_SceneBuffer = new BufferData;

		s_Data->m_Shader = CreateShader("res\\shader\\position_color_vertex.glsl", "res\\shader\\position_color_fragment.glsl");
		s_Data->m_VertexArray = CreateVertexArray();

		s_Data->vertexBuffer = CreateVertexBuffer(BufferData::maxVertex);
		s_Data->indexBuffer = CreateIndexBuffer(BufferData::maxIndexBuffer);

		// target		
		// s_Data->m_VertexArray = CreateVertexBuffer(BufferData::maxVertex);
		// s_Data->m_VertexArray = CreateIndexBuffer(BufferData::maxIndexBuffer);

		// We need a vertex and index buffer
		BufferLayout layout = s_Data->m_Shader->GetShaderAttributes();

		//BufferLayout layout({
		//	{ 0, { ShaderDataType::Vec3, "aPosition"} },
		//	{ 1, { ShaderDataType::Vec4, "aColor" } },
		//	{ 2, { ShaderDataType::Vec2, "aTexCoord" } },
		//	{ 3, { ShaderDataType::Float, "aTexIndex" } }
		//});

		s_Data->m_Shader->Bind();
		s_Data->m_VertexArray->ApplyShaderLayout(layout);

		s_Data->m_Material = CreateMaterial();
		UniformCache uniforms = s_Data->m_Shader->GetShaderUniforms();
		s_Data->m_Material->SetUniformCache(uniforms);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
		delete s_SceneBuffer;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		// testing
		void* ProjectionViewData = (void*)&camera.GetViewProjectionMatrix();
		ENGINE_ASSERT(ProjectionViewData, "The data is NULL");

		s_Data->m_Material->SetUniformData("uProjectionView", ProjectionViewData);
	}

	void Renderer2D::EndScene()
	{
		// this is temporary. Just in case we need this. Because we only use 1 shader and vertex array.
		// So, we don't need to worry about the bounded shader
		//Bind();

		s_Data->m_VertexArray->UpdateBufferData(
			s_SceneBuffer->vertexData.data(),
			s_SceneBuffer->indicesData.data(),
			BufferData::vertexIndex * sizeof(Vertex),
			BufferData::indicesIndex
		);

		Validate();

		RendererCommand::Draw(s_SceneBuffer->indicesIndex);

		Flush();
	}

	void Renderer2D::SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float texIndex)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex or BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
		{
			ENGINE_WARN("Buffer Data is full");
			return;
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::array<Vertex, 4> newQuad =
		{
			Vertex{ glm::vec3(pos.x - halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex{ glm::vec3(pos.x + halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex{ glm::vec3(pos.x + halfWidth, pos.y + halfHeight, pos.z), color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex{ glm::vec3(pos.x - halfWidth, pos.y + halfHeight, pos.z), color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		std::copy(newQuad.begin(), newQuad.end(), s_SceneBuffer->vertexData.begin() + BufferData::vertexIndex);
		BufferData::vertexIndex += 4;

		uint32_t offset = BufferData::indicesOffset;
		std::array<uint32_t, 6> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		std::copy(newQuadIndices.begin(), newQuadIndices.end(), s_SceneBuffer->indicesData.begin() + BufferData::indicesIndex);

		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;
	}

	void Renderer2D::SubmitTriangle(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float texIndex)
	{
		if (BufferData::vertexIndex + 3 >= BufferData::maxVertex || BufferData::indicesIndex + 3 >= BufferData::maxIndexBuffer)
		{
			ENGINE_WARN("Buffer Data is full");
			return;
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::array<Vertex, 3> newTriangle =
		{
			Vertex{ glm::vec3(pos.x - halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex{ glm::vec3(pos.x + halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex{ glm::vec3(pos.x, pos.y + halfHeight, pos.z), color, glm::vec2(0.5f, 1.0f), texIndex } // atas (tengah)
		};

		std::copy(newTriangle.begin(), newTriangle.end(), s_SceneBuffer->vertexData.begin() + BufferData::vertexIndex);
		BufferData::vertexIndex += 3;

		uint32_t offset = BufferData::indicesOffset;
		std::array<uint32_t, 3> newTriangleIndices =
		{
			offset,
			offset + 1,
			offset + 2
		};
		std::copy(newTriangleIndices.begin(), newTriangleIndices.end(), s_SceneBuffer->indicesData.begin() + BufferData::indicesIndex);

		BufferData::indicesIndex += 3;
		BufferData::indicesOffset += 3;
	}

	void Renderer2D::Validate()
	{
		s_Data->m_Material->ValidateMaterial();
	}

	void Renderer2D::Bind()
	{
		s_Data->m_VertexArray->Bind();
		s_Data->m_Shader->Bind();
	}

	void Renderer2D::Flush()
	{
		BufferData::vertexIndex = 0;
		BufferData::indicesIndex = 0;
		BufferData::indicesOffset = 0;
		
		// this shouldn't like this. I worry about the performance
		s_SceneBuffer->indicesData.fill(0);
		s_SceneBuffer->vertexData.fill(Vertex());
	}

}