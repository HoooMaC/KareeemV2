#include "Core/Kareeem.h"
#include "Renderer2D.h"

#include "Renderer/Material.h"

#include "Renderer/RenderCommand.h"

namespace Karem {

	RendererData* Renderer2D::s_Buffer = new RendererData;
	Renderer2DMeshes* Renderer2D::s_Meshes = new Renderer2DMeshes;

	void Renderer2D::Initialize()
	{

		s_Meshes = new Renderer2DMeshes;
		s_Buffer = new RendererData;


		s_Meshes->m_Shader = CreateShader("res/shader/vertex.shader.glsl", "res/shader/fragment.shader.glsl");
		s_Meshes->m_VertexArray = CreateVertexArray();

		s_Meshes->vertexBuffer = CreateVertexBuffer(RendererData::maxVertex * sizeof(Vertex2D));
		s_Meshes->indexBuffer = CreateIndexBuffer(RendererData::maxIndexBuffer);

		// We need a vertex and index buffer
		BufferLayout layout = s_Meshes->m_Shader->GetShaderAttributes();

		s_Meshes->m_Shader->Bind();
		s_Meshes->m_VertexArray->ApplyShaderLayout(layout);

		s_Meshes->m_Material = CreateMaterial();
		
		UniformCache uniforms = s_Meshes->m_Shader->GetShaderUniforms();
		s_Meshes->m_Material->SetUniformCache(uniforms);

		s_Buffer->TextureContainer.resize(32);
		s_Buffer->TextureSlotContainer.resize(32);

		s_Buffer->TextureContainer[0] = Karem::CreateTexture2D();

		for (int i = 0; i < 32; i++)
			s_Buffer->TextureSlotContainer[i] = i;

	}

	void Renderer2D::Shutdown()
	{
		delete s_Buffer;
		delete s_Meshes;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		// testing
		void* ProjectionViewData = (void*)&camera.GetViewProjectionMatrix();
		ENGINE_ASSERT(ProjectionViewData, "The data is NULL");
		s_Meshes->m_Material->SetUniformData("uProjectionView", ProjectionViewData);

		s_Buffer->vertexData.resize(RendererData::maxVertex);
		s_Buffer->indicesData.resize(RendererData::maxIndexBuffer);
	}

	void Renderer2D::EndScene()
	{
		s_Meshes->m_VertexArray->UpdateBufferData(
			s_Buffer->vertexData.data(),
			s_Buffer->indicesData.data(),
			RendererData::vertexIndex * sizeof(Vertex2D),
			RendererData::indicesIndex
		);

		Validate();

		RendererCommand::Draw(s_Buffer->indicesIndex);

		Flush();
	}

	void Renderer2D::SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, float texIndex)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ glm::vec4(pos.x + halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ glm::vec4(pos.x - halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_Buffer->vertexData.insert(s_Buffer->vertexData.begin() + RendererData::vertexIndex, newQuad.begin(), newQuad.end());
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_Buffer->indicesData.insert(s_Buffer->indicesData.begin() + RendererData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer2D::SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float texIndex, const glm::vec4& color)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}


		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ glm::vec4(pos.x + halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ glm::vec4(pos.x - halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_Buffer->vertexData.insert(s_Buffer->vertexData.begin() + RendererData::vertexIndex, newQuad.begin(), newQuad.end());
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_Buffer->indicesData.insert(s_Buffer->indicesData.begin() + RendererData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		s_Buffer->TextureContainer[(int)texIndex] = texture;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer2D::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, float texIndex)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ transform * glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ transform * glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f), color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ transform * glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f), color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_Buffer->vertexData.insert(s_Buffer->vertexData.begin() + RendererData::vertexIndex, newQuad.begin(), newQuad.end());
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_Buffer->indicesData.insert(s_Buffer->indicesData.begin() + RendererData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer2D::SubmitQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float texIndex, const glm::vec4& color)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ transform * glm::vec4( -0.5f, -0.5f, 0.0f, 1.0f ), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ transform * glm::vec4(  0.5f, -0.5f, 0.0f, 1.0f ), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ transform * glm::vec4(  0.5f,  0.5f, 0.0f, 1.0f ), color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ transform * glm::vec4( -0.5f,  0.5f, 0.0f, 1.0f ), color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_Buffer->vertexData.insert(s_Buffer->vertexData.begin() + RendererData::vertexIndex, newQuad.begin(), newQuad.end());
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_Buffer->indicesData.insert(s_Buffer->indicesData.begin() + RendererData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		s_Buffer->TextureContainer[(int)texIndex] = texture;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer2D::SubmitQuad(const glm::mat4& transform, const std::shared_ptr<SubTexture2D>& subTexture, float texIndex, const glm::vec4& color)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		const std::shared_ptr<Texture2D>& reference = subTexture->GetTextureReference();
		const glm::vec2* texCoord = subTexture->GetTexCoord();

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), color, *(texCoord + 0), texIndex }, // kiri bawah
			Vertex2D{ transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),  color, *(texCoord + 1), texIndex }, // kanan bawah
			Vertex2D{ transform * glm::vec4(0.5f,  0.5f, 0.0f, 1.0f),  color, *(texCoord + 2), texIndex }, // kanan atas
			Vertex2D{ transform * glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f), color, *(texCoord + 3), texIndex }  // kiri atas
		};

		s_Buffer->vertexData.insert(s_Buffer->vertexData.begin() + RendererData::vertexIndex, newQuad.begin(), newQuad.end());
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_Buffer->indicesData.insert(s_Buffer->indicesData.begin() + RendererData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		s_Buffer->TextureContainer[(int)texIndex] = reference;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer2D::SubmitSubTexturedQuad(const glm::vec4& pos, const glm::vec2& size, const std::shared_ptr<SubTexture2D>& subTexture, float texIndex, const glm::vec4& color)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		const std::shared_ptr<Texture2D>& reference = subTexture->GetTextureReference();
		const glm::vec2* texCoord = subTexture->GetTexCoord();

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, *(texCoord + 0), texIndex }, // kiri bawah
			Vertex2D{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, *(texCoord + 1), texIndex }, // kanan bawah
			Vertex2D{ glm::vec4(pos.x + halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, *(texCoord + 2), texIndex }, // kanan atas
			Vertex2D{ glm::vec4(pos.x - halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, *(texCoord + 3), texIndex }  // kiri atas
		};

		s_Buffer->vertexData.insert(s_Buffer->vertexData.begin() + RendererData::vertexIndex, newQuad.begin(), newQuad.end());
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_Buffer->indicesData.insert(s_Buffer->indicesData.begin() + RendererData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		s_Buffer->TextureContainer[(int)texIndex] = reference;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer2D::SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, const glm::vec4& color, float texIndex)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex || RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			ENGINE_WARN("Buffer Data is full");
			return;
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		// Hitung sudut-sudut setelah rotasi
		glm::vec4 bottomLeft(-halfWidth, -halfHeight, 0.0f, 1.0f);
		glm::vec4 bottomRight(halfWidth, -halfHeight, 0.0f, 1.0f);
		glm::vec4 topRight(halfWidth, halfHeight, 0.0f, 1.0f);
		glm::vec4 topLeft(-halfWidth, halfHeight, 0.0f, 1.0f);

		bottomLeft = glm::vec4(rotationMatrix * bottomLeft);
		bottomRight = glm::vec4(rotationMatrix * bottomRight);
		topRight = glm::vec4(rotationMatrix * topRight);
		topLeft = glm::vec4(rotationMatrix * topLeft);

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ bottomLeft + pos, color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ bottomRight + pos, color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ topRight + pos, color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ topLeft + pos, color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_Buffer->vertexData.insert(s_Buffer->vertexData.begin() + RendererData::vertexIndex, newQuad.begin(), newQuad.end());
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_Buffer->indicesData.insert(s_Buffer->indicesData.begin() + RendererData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer2D::SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float texIndex, const glm::vec4& color)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex || RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		// Hitung sudut-sudut setelah rotasi
		glm::vec4 bottomLeft(-halfWidth, -halfHeight, 0.0f, 1.0f);
		glm::vec4 bottomRight(halfWidth, -halfHeight, 0.0f, 1.0f);
		glm::vec4 topRight(halfWidth, halfHeight, 0.0f, 1.0f);
		glm::vec4 topLeft(-halfWidth, halfHeight, 0.0f, 1.0f);

		bottomLeft = glm::vec4(rotationMatrix * bottomLeft);
		bottomRight = glm::vec4(rotationMatrix * bottomRight);
		topRight = glm::vec4(rotationMatrix * topRight);
		topLeft = glm::vec4(rotationMatrix * topLeft);

		std::vector<Vertex2D> newQuad =
		{
			Vertex2D{ bottomLeft + pos, color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ bottomRight + pos, color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ topRight + pos, color, glm::vec2(1.0f, 1.0f), texIndex }, // kanan atas
			Vertex2D{ topLeft + pos, color, glm::vec2(0.0f, 1.0f), texIndex }  // kiri atas
		};

		s_Buffer->vertexData.insert(s_Buffer->vertexData.begin() + RendererData::vertexIndex, newQuad.begin(), newQuad.end());
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		std::vector<uint32_t> newQuadIndices =
		{
			offset,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset
		};
		s_Buffer->indicesData.insert(s_Buffer->indicesData.begin() + RendererData::indicesIndex, newQuadIndices.begin(), newQuadIndices.end());

		s_Buffer->TextureContainer[(int)texIndex] = texture;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer2D::SubmitTriangle(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, float texIndex)
	{
		if (RendererData::vertexIndex + 3 >= RendererData::maxVertex || RendererData::indicesIndex + 3 >= RendererData::maxIndexBuffer)
		{
			ENGINE_WARN("Buffer Data is full");
			return;
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		std::vector<Vertex2D> newTriangle =
		{
			Vertex2D{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex }, // kiri bawah
			Vertex2D{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 0.0f), texIndex }, // kanan bawah
			Vertex2D{ glm::vec4(pos.x, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(0.5f, 1.0f), texIndex } // atas (tengah)
		};

		s_Buffer->vertexData.insert(s_Buffer->vertexData.begin() + RendererData::vertexIndex, newTriangle.begin(), newTriangle.end());
		RendererData::vertexIndex += 3;

		uint32_t offset = RendererData::indicesOffset;
		std::vector<uint32_t> newTriangleIndices =
		{
			offset,
			offset + 1,
			offset + 2
		};
		s_Buffer->indicesData.insert(s_Buffer->indicesData.begin() + RendererData::indicesIndex, newTriangleIndices.begin(), newTriangleIndices.end());

		RendererData::indicesIndex += 3;
		RendererData::indicesOffset += 3;
	}

	void Renderer2D::Validate()
	{
		s_Meshes->m_Material->SetUniformData("uTexture", (void*)s_Buffer->TextureSlotContainer.data());
		s_Meshes->m_Material->ValidateMaterial();

		for (const auto& texture : s_Buffer->TextureContainer)
		{
			if (texture == nullptr)
				continue;
			texture->Bind();
		}
	}

	void Renderer2D::Flush()
	{
		RendererData::vertexIndex = 0;
		RendererData::indicesIndex = 0;
		RendererData::indicesOffset = 0;

		s_Buffer->indicesData.clear();
		s_Buffer->vertexData.clear();
	}

	void Renderer2D::Bind()
	{
		s_Meshes->m_VertexArray->Bind();
		s_Meshes->m_Shader->Bind();
	}

}