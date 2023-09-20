#include "Core/Kareeem.h"
#include "Renderer2D.h"

#include "Core/CoreFunction.h"

#include "Renderer/OrthographicCamera.h"

#include "Renderer/Material.h"
#include "Renderer/BaseShader.h"
#include "Renderer/BaseBuffer.h"
#include "Renderer/BaseTexure.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/SubTexture.h"

#include <glm/glm.hpp>

namespace Karem {

	struct Vertex2D
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		Vertex2D()
			: Position(glm::vec3(0.0f)), Color(glm::vec4(0.0f)), TexCoord(glm::vec2(0.0f)), TexIndex(0.0f) {}

		Vertex2D(const glm::vec3& position, const glm::vec4& color, const glm::vec2& texCoord, float texIndex)
			: Position(position), Color(color), TexCoord(texCoord), TexIndex(texIndex) {}
	};

	struct BufferData
	{
		static constexpr uint64_t maxVertex = 1024;
		static constexpr uint64_t maxIndexBuffer = maxVertex * 6;

		// this is need to change with array, but not std::array
		std::vector<Vertex2D> vertexData;
		std::vector<uint32_t> indicesData;

		std::vector<std::shared_ptr<Texture2D>>TextureContainer;
		std::vector<uint32_t> TextureSlotContainer;

		static uint32_t vertexIndex;
		static uint32_t indicesIndex;
		static uint32_t indicesOffset;
	};

	uint32_t BufferData::vertexIndex = 0;
	uint32_t BufferData::indicesIndex = 0;
	uint32_t BufferData::indicesOffset = 0;

	struct Renderer2DData
	{
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Material> m_Material;

		// this is temporary
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};

	static Renderer2DData* s_Data;
	static BufferData* s_SceneBuffer;

	void Renderer2D::Initialize()
	{
		s_Data = new Renderer2DData;
		s_SceneBuffer = new BufferData;


		s_Data->m_Shader = CreateShader("res/shader/vertex.shader.glsl", "res/shader/fragment.shader.glsl");
		s_Data->m_VertexArray = CreateVertexArray();

		s_Data->vertexBuffer = CreateVertexBuffer(BufferData::maxVertex * sizeof(Vertex2D));
		s_Data->indexBuffer = CreateIndexBuffer(BufferData::maxIndexBuffer);

		// We need a vertex and index buffer
		BufferLayout layout = s_Data->m_Shader->GetShaderAttributes();

		s_Data->m_Shader->Bind();
		s_Data->m_VertexArray->ApplyShaderLayout(layout);

		s_Data->m_Material = CreateMaterial();
		UniformCache uniforms = s_Data->m_Shader->GetShaderUniforms();
		s_Data->m_Material->SetUniformCache(uniforms);

		s_SceneBuffer->TextureContainer.resize(32);
		s_SceneBuffer->TextureSlotContainer.resize(32);

		s_SceneBuffer->TextureContainer[0] = Karem::CreateTexture2D();

		for(int i = 0; i < 32; i++)
			s_SceneBuffer->TextureSlotContainer[i] = i;

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

		s_SceneBuffer->vertexData.resize(BufferData::maxVertex);
		s_SceneBuffer->indicesData.resize(BufferData::maxIndexBuffer);
	}

	void Renderer2D::EndScene()
	{
		s_Data->m_VertexArray->UpdateBufferData(
			s_SceneBuffer->vertexData.data(),
			s_SceneBuffer->indicesData.data(),
			BufferData::vertexIndex * sizeof(Vertex2D),
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
			EndScene();
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ glm::vec3(pos.x - halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ glm::vec3(pos.x + halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ glm::vec3(pos.x + halfWidth, pos.y + halfHeight, pos.z), color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ glm::vec3(pos.x - halfWidth, pos.y + halfHeight, pos.z), color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_SceneBuffer->vertexData.insert(s_SceneBuffer->vertexData.begin() + BufferData::vertexIndex, newQuad.begin(), newQuad.end());
		BufferData::vertexIndex += 4;

		uint32_t offset = BufferData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_SceneBuffer->indicesData.insert(s_SceneBuffer->indicesData.begin() + BufferData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;
	}

	void Renderer2D::SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float texIndex, const glm::vec4& color)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex or BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
		{
			EndScene();
		}


		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ glm::vec3(pos.x - halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ glm::vec3(pos.x + halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ glm::vec3(pos.x + halfWidth, pos.y + halfHeight, pos.z), color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ glm::vec3(pos.x - halfWidth, pos.y + halfHeight, pos.z), color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_SceneBuffer->vertexData.insert(s_SceneBuffer->vertexData.begin() + BufferData::vertexIndex, newQuad.begin(), newQuad.end());
		BufferData::vertexIndex += 4;

		uint32_t offset = BufferData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_SceneBuffer->indicesData.insert(s_SceneBuffer->indicesData.begin() + BufferData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		s_SceneBuffer->TextureContainer[(int)texIndex] = texture;

		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;
	}

	void Renderer2D::SubmitSubTexturedQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subTexture, float texIndex, const glm::vec4& color)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex or BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
		{
			EndScene();
		}

		const std::shared_ptr<Texture2D>& reference = subTexture->GetTextureReference();
		const glm::vec2* texCoord = subTexture->GetTexCoord();

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ glm::vec3(pos.x - halfWidth, pos.y - halfHeight, pos.z), color, *(texCoord + 0), texIndex }, // kiri bawah
			Vertex2D{ glm::vec3(pos.x + halfWidth, pos.y - halfHeight, pos.z), color, *(texCoord + 1), texIndex }, // kanan bawah
			Vertex2D{ glm::vec3(pos.x + halfWidth, pos.y + halfHeight, pos.z), color, *(texCoord + 2), texIndex }, // kanan atas
			Vertex2D{ glm::vec3(pos.x - halfWidth, pos.y + halfHeight, pos.z), color, *(texCoord + 3), texIndex }  // kiri atas
		};

		s_SceneBuffer->vertexData.insert(s_SceneBuffer->vertexData.begin() + BufferData::vertexIndex, newQuad.begin(), newQuad.end());
		BufferData::vertexIndex += 4;

		uint32_t offset = BufferData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_SceneBuffer->indicesData.insert(s_SceneBuffer->indicesData.begin() + BufferData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		s_SceneBuffer->TextureContainer[(int)texIndex] = reference;

		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;
	}

	void Renderer2D::SubmitRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color, float texIndex)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex || BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
		{
			ENGINE_WARN("Buffer Data is full");
			return;
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		// Hitung sudut-sudut setelah rotasi
		glm::vec3 bottomLeft(	-halfWidth,	-halfHeight,	0.0f);
		glm::vec3 bottomRight(	halfWidth,	-halfHeight,	0.0f);
		glm::vec3 topRight(		halfWidth,	halfHeight,		0.0f);
		glm::vec3 topLeft(		-halfWidth,	halfHeight,		0.0f);

		bottomLeft = glm::vec3(rotationMatrix * glm::vec4(bottomLeft, 1.0f));
		bottomRight = glm::vec3(rotationMatrix * glm::vec4(bottomRight, 1.0f));
		topRight = glm::vec3(rotationMatrix * glm::vec4(topRight, 1.0f));
		topLeft = glm::vec3(rotationMatrix * glm::vec4(topLeft, 1.0f));

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ bottomLeft + pos, color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ bottomRight + pos, color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ topRight + pos, color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ topLeft + pos, color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_SceneBuffer->vertexData.insert(s_SceneBuffer->vertexData.begin() + BufferData::vertexIndex, newQuad.begin(), newQuad.end());
		BufferData::vertexIndex += 4;

		uint32_t offset = BufferData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_SceneBuffer->indicesData.insert(s_SceneBuffer->indicesData.begin() + BufferData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;
	}

	void Renderer2D::SubmitRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float texIndex, const glm::vec4& color)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex || BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
		{
			EndScene();
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		// Hitung sudut-sudut setelah rotasi
		glm::vec3 bottomLeft(-halfWidth, -halfHeight, 0.0f);
		glm::vec3 bottomRight(halfWidth, -halfHeight, 0.0f);
		glm::vec3 topRight(halfWidth, halfHeight, 0.0f);
		glm::vec3 topLeft(-halfWidth, halfHeight, 0.0f);

		bottomLeft = glm::vec3(rotationMatrix * glm::vec4(bottomLeft, 1.0f));
		bottomRight = glm::vec3(rotationMatrix * glm::vec4(bottomRight, 1.0f));
		topRight = glm::vec3(rotationMatrix * glm::vec4(topRight, 1.0f));
		topLeft = glm::vec3(rotationMatrix * glm::vec4(topLeft, 1.0f));

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ bottomLeft + pos, color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ bottomRight + pos, color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ topRight + pos, color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ topLeft + pos, color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_SceneBuffer->vertexData.insert(s_SceneBuffer->vertexData.begin() + BufferData::vertexIndex, newQuad.begin(), newQuad.end());
		BufferData::vertexIndex += 4;

		uint32_t offset = BufferData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_SceneBuffer->indicesData.insert(s_SceneBuffer->indicesData.begin() + BufferData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		s_SceneBuffer->TextureContainer[(int)texIndex] = texture;

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

		std::vector<Vertex2D> newTriangle =
		{
			Vertex2D{ glm::vec3(pos.x - halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ glm::vec3(pos.x + halfWidth, pos.y - halfHeight, pos.z), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ glm::vec3(pos.x, pos.y + halfHeight, pos.z), color, glm::vec2(0.5f, 1.0f), texIndex } // atas (tengah)
		};

		s_SceneBuffer->vertexData.insert(s_SceneBuffer->vertexData.begin() + BufferData::vertexIndex, newTriangle.begin(), newTriangle.end());
		BufferData::vertexIndex += 3;

		uint32_t offset = BufferData::indicesOffset;
		std::vector<uint32_t> newTriangleIndices =
		{
			offset,
			offset + 1,
			offset + 2
		};
		s_SceneBuffer->indicesData.insert(s_SceneBuffer->indicesData.begin() + BufferData::indicesIndex, newTriangleIndices.begin(), newTriangleIndices.end());

		BufferData::indicesIndex += 3;
		BufferData::indicesOffset += 3;
	}

	void Renderer2D::Validate()
	{
		s_Data->m_Material->SetUniformData("uTexture", (void*)s_SceneBuffer->TextureSlotContainer.data());
		s_Data->m_Material->ValidateMaterial();

		for (const auto& texture : s_SceneBuffer->TextureContainer)
		{
			if (texture == nullptr)
				continue;
			texture->Bind();
		}
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

		s_SceneBuffer->indicesData.clear();
		s_SceneBuffer->vertexData.clear();
	}

}
