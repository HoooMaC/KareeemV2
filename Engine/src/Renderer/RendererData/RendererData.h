#pragma once

namespace Karem {

	class Texture2D;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Material;
	class Shader;

	struct Vertex2D
	{
		glm::vec4 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		Vertex2D()
			: Position(glm::vec4(0.0f)), Color(glm::vec4(0.0f)), TexCoord(glm::vec2(0.0f)), TexIndex(0.0f) {}

		Vertex2D(const glm::vec4& position, const glm::vec4& color, const glm::vec2& texCoord, float texIndex)
			: Position(position), Color(color), TexCoord(texCoord), TexIndex(texIndex) {}
	};

	struct RendererData
	{
		static constexpr uint64_t maxVertex = 1024;
		static constexpr uint64_t maxIndexBuffer = maxVertex * 6;

		// this is need to change with array, but not std::array
		std::vector<Vertex2D> vertexData; // size =  max vertex
		std::vector<uint32_t> indicesData; // size = max Index Buffer

		std::vector<std::shared_ptr<Texture2D>>TextureContainer; // size = 32
		std::vector<uint32_t> TextureSlotContainer; // size = 32

		static uint32_t vertexIndex;
		static uint32_t indicesIndex;
		static uint32_t indicesOffset;
	};


	struct Renderer2DMeshes
	{
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Material> m_Material;

		// this is temporary
		std::shared_ptr<VertexBuffer> vertexBuffer;		
		std::shared_ptr<IndexBuffer> indexBuffer;
	};

}