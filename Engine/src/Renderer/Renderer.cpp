#include "Core/Kareeem.h"
#include "Renderer.h"

#include "Renderer/Material.h"

#include "Renderer/RenderCommand.h"

namespace Karem {

	RendererData* Renderer::s_Buffer = new RendererData;
	RendererMeshes* Renderer::s_Meshes = new RendererMeshes;
	static constinit glm::mat4 s_ViewProjection = glm::mat4(1.0f);

	// this is temporary
	const std::vector<glm::vec4> defaultQuadPosition =
	{
		{ -0.5f, -0.5f, 1.0f, 1.0f },
		{  0.5f, -0.5f, 1.0f, 1.0f },
		{  0.5f,  0.5f, 1.0f, 1.0f },
		{ -0.5f,  0.5f, 1.0f, 1.0f }
	};

	void Renderer::Initialize()
	{

		s_Meshes = new RendererMeshes;
		s_Buffer = new RendererData;


		s_Meshes->m_Shader = CreateShader("res/shader/vertex.shader.glsl", "res/shader/fragment.shader.glsl");
		s_Meshes->m_VertexArray = CreateVertexArray();

		s_Meshes->vertexBuffer = CreateVertexBuffer(RendererData::maxVertex * sizeof(Vertex));
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

		s_Buffer->TextureContainer[0] = Karem::CreateTexture2D(0);

		for (int i = 0; i < 32; i++)
			s_Buffer->TextureSlotContainer[i] = i;

	}

	void Renderer::Shutdown()
	{
		delete s_Buffer;
		delete s_Meshes;
	}

	void Renderer::BeginScene(const glm::mat4& projection, const glm::mat4& cameraTransform)
	{
		s_ViewProjection = projection * glm::inverse(cameraTransform);
		s_Meshes->m_Material->SetUniformData("uProjectionView", (void*)glm::value_ptr(s_ViewProjection));

		s_Buffer->vertexData.resize(RendererData::maxVertex);
		s_Buffer->indicesData.resize(RendererData::maxIndexBuffer);
	}

	void Renderer::EndScene()
	{
		s_Meshes->m_VertexArray->UpdateBufferData(
			s_Buffer->vertexData.data(),
			s_Buffer->indicesData.data(),
			RendererData::vertexIndex * sizeof(Vertex),
			RendererData::indicesIndex
		);

		Validate();

		RendererCommand::Draw(s_Buffer->indicesIndex);

		Flush();
	}

	void Renderer::SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, float texIndex)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		s_Buffer->vertexData[RendererData::vertexIndex + 0] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 1] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 2] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 1.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 3] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 1.0f), texIndex };
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		s_Buffer->indicesData[RendererData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[RendererData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[RendererData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[RendererData::indicesIndex + 5] = offset + 0;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer::SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float texIndex, const glm::vec4& color)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}


		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		s_Buffer->vertexData[RendererData::vertexIndex + 0] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 1] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 2] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 1.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 3] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 1.0f), texIndex };
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		s_Buffer->indicesData[RendererData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[RendererData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[RendererData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[RendererData::indicesIndex + 5] = offset + 0;

		s_Buffer->TextureContainer[(int)texIndex] = texture;
		//s_Buffer->TextureSlotContainer[(int)texIndex] = texture->GetSlot();
		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, float texIndex)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		s_Buffer->vertexData[RendererData::vertexIndex + 0] = Vertex{ transform * defaultQuadPosition[0], color, glm::vec2(0.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 1] = Vertex{ transform * defaultQuadPosition[1], color, glm::vec2(1.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 2] = Vertex{ transform * defaultQuadPosition[2], color, glm::vec2(1.0f, 1.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 3] = Vertex{ transform * defaultQuadPosition[3], color, glm::vec2(0.0f, 1.0f), texIndex };
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		s_Buffer->indicesData[RendererData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[RendererData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[RendererData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[RendererData::indicesIndex + 5] = offset + 0;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer::SubmitQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float texIndex, const glm::vec4& color)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		s_Buffer->vertexData[RendererData::vertexIndex + 0] = Vertex{ transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 1] = Vertex{ transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),  color, glm::vec2(1.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 2] = Vertex{ transform * glm::vec4(0.5f,  0.5f, 0.0f, 1.0f),  color, glm::vec2(1.0f, 1.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 3] = Vertex{ transform * glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f), color, glm::vec2(0.0f, 1.0f), texIndex };
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		s_Buffer->indicesData[RendererData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[RendererData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[RendererData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[RendererData::indicesIndex + 5] = offset + 0;

		s_Buffer->TextureContainer[(int)texIndex] = texture;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer::SubmitQuad(const glm::mat4& transform, const SubTexture2D& subTexture, float texIndex, const glm::vec4& color)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		const std::shared_ptr<Texture2D>& reference = subTexture.GetTextureReference();
		const glm::vec2* texCoord = subTexture.GetTexCoord();

		s_Buffer->vertexData[RendererData::vertexIndex + 0] = Vertex{ transform * defaultQuadPosition[0], color, *(texCoord + 0), texIndex }; // kiri bawah;
		s_Buffer->vertexData[RendererData::vertexIndex + 1] = Vertex{ transform * defaultQuadPosition[1], color, *(texCoord + 1), texIndex }; // kiri bawah;
		s_Buffer->vertexData[RendererData::vertexIndex + 2] = Vertex{ transform * defaultQuadPosition[2], color, *(texCoord + 2), texIndex }; // kiri bawah;
		s_Buffer->vertexData[RendererData::vertexIndex + 3] = Vertex{ transform * defaultQuadPosition[3], color, *(texCoord + 3), texIndex }; // kiri bawah;
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		s_Buffer->indicesData[RendererData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[RendererData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[RendererData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[RendererData::indicesIndex + 5] = offset + 0;

		s_Buffer->TextureContainer[(int)texIndex] = reference;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer::SubmitSubTexturedQuad(const glm::vec4& pos, const glm::vec2& size, const SubTexture2D& subTexture, float texIndex, const glm::vec4& color)
	{
		if (RendererData::vertexIndex + 4 >= RendererData::maxVertex or RendererData::indicesIndex + 6 >= RendererData::maxIndexBuffer)
		{
			EndScene();
		}

		const std::shared_ptr<Texture2D>& reference = subTexture.GetTextureReference();
		const glm::vec2* texCoord = subTexture.GetTexCoord();

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		s_Buffer->vertexData[RendererData::vertexIndex + 0] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, *(texCoord + 0), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 1] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, *(texCoord + 1), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 2] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, *(texCoord + 2), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 3] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, *(texCoord + 3), texIndex };
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		s_Buffer->indicesData[RendererData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[RendererData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[RendererData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[RendererData::indicesIndex + 5] = offset + 0;

		s_Buffer->TextureContainer[(int)texIndex] = reference;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer::SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, const glm::vec4& color, float texIndex)
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

		s_Buffer->vertexData[RendererData::vertexIndex + 0] = Vertex{ bottomLeft + pos,  color, glm::vec2(0.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 1] = Vertex{ bottomRight + pos, color, glm::vec2(1.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 2] = Vertex{ topRight + pos,    color, glm::vec2(1.0f, 1.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 3] = Vertex{ topLeft + pos,     color, glm::vec2(0.0f, 1.0f), texIndex };
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		s_Buffer->indicesData[RendererData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[RendererData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[RendererData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[RendererData::indicesIndex + 5] = offset + 0;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer::SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float texIndex, const glm::vec4& color)
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

		s_Buffer->vertexData[RendererData::vertexIndex + 0] = Vertex{ bottomLeft + pos,  color, glm::vec2(0.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 1] = Vertex{ bottomRight + pos, color, glm::vec2(1.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 2] = Vertex{ topRight + pos,    color, glm::vec2(1.0f, 1.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 3] = Vertex{ topLeft + pos,     color, glm::vec2(0.0f, 1.0f), texIndex };
		RendererData::vertexIndex += 4;

		uint32_t offset = RendererData::indicesOffset;
		s_Buffer->indicesData[RendererData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[RendererData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[RendererData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[RendererData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[RendererData::indicesIndex + 5] = offset + 0;

		s_Buffer->TextureContainer[(int)texIndex] = texture;

		RendererData::indicesIndex += 6;
		RendererData::indicesOffset += 4;
	}

	void Renderer::SubmitTriangle(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, float texIndex)
	{
		if (RendererData::vertexIndex + 3 >= RendererData::maxVertex || RendererData::indicesIndex + 3 >= RendererData::maxIndexBuffer)
		{
			ENGINE_WARN("Buffer Data is full");
			return;
		}

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		s_Buffer->vertexData[RendererData::vertexIndex + 0] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 1] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 0.0f), texIndex };
		s_Buffer->vertexData[RendererData::vertexIndex + 2] = Vertex{ glm::vec4(pos.x, pos.y + halfHeight, pos.z, 1.0f),             color, glm::vec2(0.5f, 1.0f), texIndex };
		RendererData::vertexIndex += 3;

		uint32_t offset = RendererData::indicesOffset;
		s_Buffer->indicesData[RendererData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[RendererData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[RendererData::indicesIndex + 2] = offset + 2;

		RendererData::indicesIndex += 3;
		RendererData::indicesOffset += 3;
	}

	void Renderer::Validate()
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

	void Renderer::Flush()
	{
		RendererData::vertexIndex = 0;
		RendererData::indicesIndex = 0;
		RendererData::indicesOffset = 0;

		s_Buffer->indicesData.clear();
		s_Buffer->vertexData.clear();
	}

	void Renderer::Bind()
	{
		s_Meshes->m_VertexArray->Bind();
		s_Meshes->m_Shader->Bind();
	}

}