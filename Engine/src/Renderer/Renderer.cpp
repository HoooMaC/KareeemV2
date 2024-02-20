#include "Core/Kareeem.h"
#include "Renderer.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/EditorCamera.h"

namespace Karem {

	uint32_t Renderer::BufferData::vertexIndex = 0;
	uint32_t Renderer::BufferData::indicesIndex = 0;
	uint32_t Renderer::BufferData::indicesOffset = 0;

	// TODO::CHECK THIS
	Renderer::BufferData* Renderer::s_Buffer = new BufferData;
	Renderer::Meshes* Renderer::s_Meshes = new Meshes;

	static constinit glm::mat4 s_ViewProjection = glm::mat4(1.0f);
	static constinit Texture2D* s_WhiteTexture = nullptr;

	static constexpr glm::vec4 defaultQuadPosition[4] =
	{
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	void Renderer::Initialize()
	{
		// TODO::CHECK THIS
		s_Meshes = new Meshes;
		s_Buffer = new BufferData;

		s_Meshes->m_Shader = CreateShader("res/shader/vertex.shader.glsl", "res/shader/fragment.shader.glsl");

		s_Meshes->m_VertexArray = CreateVertexArray();
		s_Meshes->vertexBuffer = CreateVertexBuffer(BufferData::maxVertex * sizeof(Vertex));
		s_Meshes->indexBuffer = CreateIndexBuffer(BufferData::maxIndexBuffer);

		{
			// We need a vertex and index buffer
			BufferLayout layout = s_Meshes->m_Shader->GetShaderAttributes();
			s_Meshes->m_Shader->Bind();
			s_Meshes->m_VertexArray->ApplyShaderLayout(layout);
		}

		{
			UniformCache uniforms = s_Meshes->m_Shader->GetShaderUniforms();
			s_Meshes->m_Material.SetUniformCache(uniforms);
			s_Meshes->m_Material.SetUniformData("uProjectionView", (void*)glm::value_ptr(s_ViewProjection));
		}

		s_Buffer->TextureContainer.resize(32);
		s_Buffer->TextureSlotContainer.resize(32);
		s_WhiteTexture = CreateRawTexture2D(0);
		s_Buffer->TextureContainer[0] = s_WhiteTexture;

		for (int i = 0; i < 32; i++)
			s_Buffer->TextureSlotContainer[i] = i;
	}

	void Renderer::Shutdown()
	{
		DestroyRawTexture2D(s_WhiteTexture);
		delete s_Buffer;
		delete s_Meshes;
	}

	void Renderer::BeginScene(EditorCamera& camera)
	{
		s_ViewProjection = camera.GetViewProjectionMatrix();

		s_Buffer->vertexData.resize(BufferData::maxVertex);
		s_Buffer->indicesData.resize(BufferData::maxIndexBuffer);
		s_Buffer->TextureContainer.resize(32);
	}

	void Renderer::BeginScene(const glm::mat4& projection, const glm::mat4& cameraTransform)
	{
		s_ViewProjection = projection * glm::inverse(cameraTransform);

		s_Buffer->vertexData.resize(BufferData::maxVertex);
		s_Buffer->indicesData.resize(BufferData::maxIndexBuffer);
		s_Buffer->TextureContainer.resize(32);
	}

	void Renderer::EndScene()
	{
		s_Meshes->m_VertexArray->UpdateBufferData(
			s_Buffer->vertexData.data(),
			s_Buffer->indicesData.data(),
			BufferData::vertexIndex * sizeof(Vertex),
			BufferData::indicesIndex
		);

		Validate();

		RendererCommand::Draw(s_Buffer->indicesIndex);

		Flush();
		s_Buffer->TextureContainer.clear();
	}

	void Renderer::SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, int entityId, int texIndex)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex or BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
		{
			EndScene();
		}
		uint32_t offset = BufferData::indicesOffset;

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		s_Buffer->vertexData[BufferData::vertexIndex + 0] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 1] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 2] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 1.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 3] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 1.0f), texIndex, entityId };

		s_Buffer->indicesData[BufferData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[BufferData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[BufferData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[BufferData::indicesIndex + 5] = offset + 0;

		BufferData::vertexIndex += 4;
		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;
	}

	void Renderer::SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, int entityId, int texIndex, const glm::vec4& color)
	{
		SubmitQuad(pos, size, color, entityId, texIndex);

		s_Buffer->TextureContainer[(int)texIndex] = texture;
	}

	void Renderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, int entityId, int texIndex)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex or BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
		{
			EndScene();
		}

		uint32_t offset = BufferData::indicesOffset;

		s_Buffer->vertexData[BufferData::vertexIndex + 0] = Vertex{ transform * defaultQuadPosition[0], color, glm::vec2(0.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 1] = Vertex{ transform * defaultQuadPosition[1], color, glm::vec2(1.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 2] = Vertex{ transform * defaultQuadPosition[2], color, glm::vec2(1.0f, 1.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 3] = Vertex{ transform * defaultQuadPosition[3], color, glm::vec2(0.0f, 1.0f), texIndex, entityId };

		s_Buffer->indicesData[BufferData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[BufferData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[BufferData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[BufferData::indicesIndex + 5] = offset + 0;

		BufferData::vertexIndex += 4;
		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;
	}

	void Renderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const Rahman::SmartRef<Texture2D> texture, int texIndex, int entityId)
	{
		SubmitQuad(transform, color, entityId, texIndex);

		if(texture)
			s_Buffer->TextureContainer[(int)texIndex] = texture;
	}

	void Renderer::SubmitQuad(const glm::mat4& transform, const SubTexture2D& subTexture, int entityId, int texIndex, const glm::vec4& color)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex or BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
		{
			EndScene();
		}

		const glm::vec2* texCoord = subTexture.GetTexCoord();

		uint32_t offset = BufferData::indicesOffset;

		s_Buffer->vertexData[BufferData::vertexIndex + 0] = Vertex{ transform * defaultQuadPosition[0], color, *(texCoord + 0), texIndex, entityId }; // kiri bawah;
		s_Buffer->vertexData[BufferData::vertexIndex + 1] = Vertex{ transform * defaultQuadPosition[1], color, *(texCoord + 1), texIndex, entityId }; // kiri bawah;
		s_Buffer->vertexData[BufferData::vertexIndex + 2] = Vertex{ transform * defaultQuadPosition[2], color, *(texCoord + 2), texIndex, entityId }; // kiri bawah;
		s_Buffer->vertexData[BufferData::vertexIndex + 3] = Vertex{ transform * defaultQuadPosition[3], color, *(texCoord + 3), texIndex, entityId }; // kiri bawah;

		s_Buffer->indicesData[BufferData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[BufferData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[BufferData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[BufferData::indicesIndex + 5] = offset + 0;

		BufferData::vertexIndex += 4;
		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;

		s_Buffer->TextureContainer[(int)texIndex] = subTexture.GetTextureReference();
	}

	void Renderer::SubmitSubTexturedQuad(const glm::vec4& pos, const glm::vec2& size, const SubTexture2D& subTexture, int entityId, int texIndex, const glm::vec4& color)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex or BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
		{
			EndScene();
		}

		const glm::vec2* texCoord = subTexture.GetTexCoord();

		uint32_t offset = BufferData::indicesOffset;

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		s_Buffer->vertexData[BufferData::vertexIndex + 0] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, *(texCoord + 0), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 1] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, *(texCoord + 1), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 2] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, *(texCoord + 2), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 3] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y + halfHeight, pos.z, 1.0f), color, *(texCoord + 3), texIndex, entityId };

		s_Buffer->indicesData[BufferData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[BufferData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[BufferData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[BufferData::indicesIndex + 5] = offset + 0;

		BufferData::vertexIndex += 4;
		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;

		s_Buffer->TextureContainer[(int)texIndex] = subTexture.GetTextureReference();
	}

	void Renderer::SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, const glm::vec4& color, int entityId, int texIndex)
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
		glm::vec4 bottomLeft(-halfWidth, -halfHeight, 0.0f, 1.0f);
		glm::vec4 bottomRight(halfWidth, -halfHeight, 0.0f, 1.0f);
		glm::vec4 topRight(halfWidth, halfHeight, 0.0f, 1.0f);
		glm::vec4 topLeft(-halfWidth, halfHeight, 0.0f, 1.0f);

		uint32_t offset = BufferData::indicesOffset;

		bottomLeft = glm::vec4(rotationMatrix * bottomLeft);
		bottomRight = glm::vec4(rotationMatrix * bottomRight);
		topRight = glm::vec4(rotationMatrix * topRight);
		topLeft = glm::vec4(rotationMatrix * topLeft);

		s_Buffer->vertexData[BufferData::vertexIndex + 0] = Vertex{ bottomLeft + pos,  color, glm::vec2(0.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 1] = Vertex{ bottomRight + pos, color, glm::vec2(1.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 2] = Vertex{ topRight + pos,    color, glm::vec2(1.0f, 1.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 3] = Vertex{ topLeft + pos,     color, glm::vec2(0.0f, 1.0f), texIndex, entityId };

		s_Buffer->indicesData[BufferData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[BufferData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[BufferData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[BufferData::indicesIndex + 5] = offset + 0;

		BufferData::vertexIndex += 4;
		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;
	}

	void Renderer::SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, Rahman::SmartRef<Texture2D> texture, int entityId, int texIndex, const glm::vec4& color)
	{
		if (BufferData::vertexIndex + 4 >= BufferData::maxVertex || BufferData::indicesIndex + 6 >= BufferData::maxIndexBuffer)
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

		uint32_t offset = BufferData::indicesOffset;

		bottomLeft = glm::vec4(rotationMatrix * bottomLeft);
		bottomRight = glm::vec4(rotationMatrix * bottomRight);
		topRight = glm::vec4(rotationMatrix * topRight);
		topLeft = glm::vec4(rotationMatrix * topLeft);

		s_Buffer->vertexData[BufferData::vertexIndex + 0] = Vertex{ bottomLeft + pos,  color, glm::vec2(0.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 1] = Vertex{ bottomRight + pos, color, glm::vec2(1.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 2] = Vertex{ topRight + pos,    color, glm::vec2(1.0f, 1.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 3] = Vertex{ topLeft + pos,     color, glm::vec2(0.0f, 1.0f), texIndex, entityId };

		s_Buffer->indicesData[BufferData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[BufferData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[BufferData::indicesIndex + 2] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 3] = offset + 2;
		s_Buffer->indicesData[BufferData::indicesIndex + 4] = offset + 3;
		s_Buffer->indicesData[BufferData::indicesIndex + 5] = offset + 0;

		BufferData::vertexIndex += 4;
		BufferData::indicesIndex += 6;
		BufferData::indicesOffset += 4;

		s_Buffer->TextureContainer[(int)texIndex] = texture;
	}

	void Renderer::SubmitTriangle(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, int entityId, int texIndex)
	{
		if (BufferData::vertexIndex + 3 >= BufferData::maxVertex || BufferData::indicesIndex + 3 >= BufferData::maxIndexBuffer)
		{
			ENGINE_WARN("Buffer Data is full");
			return;
		}

		uint32_t offset = BufferData::indicesOffset;

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		s_Buffer->vertexData[BufferData::vertexIndex + 0] = Vertex{ glm::vec4(pos.x - halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(0.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 1] = Vertex{ glm::vec4(pos.x + halfWidth, pos.y - halfHeight, pos.z, 1.0f), color, glm::vec2(1.0f, 0.0f), texIndex, entityId };
		s_Buffer->vertexData[BufferData::vertexIndex + 2] = Vertex{ glm::vec4(pos.x, pos.y + halfHeight, pos.z, 1.0f),             color, glm::vec2(0.5f, 1.0f), texIndex, entityId };

		s_Buffer->indicesData[BufferData::indicesIndex + 0] = offset + 0;
		s_Buffer->indicesData[BufferData::indicesIndex + 1] = offset + 1;
		s_Buffer->indicesData[BufferData::indicesIndex + 2] = offset + 2;

		BufferData::vertexIndex += 3;
		BufferData::indicesIndex += 3;
		BufferData::indicesOffset += 3;
	}

	void Renderer::Validate()
	{
		s_Meshes->m_Material.SetUniformData("uTexture", (void*)s_Buffer->TextureSlotContainer.data());
		s_Meshes->m_Material.ValidateMaterial();

		for (const auto& texture : s_Buffer->TextureContainer)
		{
			try {
				if(texture.IsValid())
					texture->Bind();
			}
			catch (...) {
				ENGINE_ASSERT(false, "");
			}
		}
	}

	void Renderer::Flush()
	{
		BufferData::vertexIndex = 0;
		BufferData::indicesIndex = 0;
		BufferData::indicesOffset = 0;

		s_Buffer->indicesData.clear();
		s_Buffer->vertexData.clear();
	}

	void Renderer::Bind()
	{
		s_Meshes->m_VertexArray->Bind();
		s_Meshes->m_Shader->Bind();
	}

}