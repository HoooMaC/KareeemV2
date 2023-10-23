#pragma once

#include "Renderer/BaseBuffer.h"
#include "Renderer/BaseTexture.h"
#include "Renderer/SubTexture.h"
#include "Renderer/Material.h"

#include <glm/glm.hpp>

namespace Karem {

	class Renderer
	{
	public:
		static void Initialize();
		static void Shutdown();

		/*Material material*/
		static void BeginScene(const glm::mat4& projection, const glm::mat4& cameraTransform);
		static void EndScene();

		static void SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, float texIndex = 0.0f);
		static void SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float texIndex = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color, float texIndex = 0.0f);
		static void SubmitQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float texIndex = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void SubmitQuad(const glm::mat4& transform, const SubTexture2D& subTexture, float texIndex = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

		static void SubmitSubTexturedQuad(const glm::vec4& pos, const glm::vec2& size, const SubTexture2D& subTexture, float texIndex = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

		static void SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, const glm::vec4& color, float texIndex = 0.0f);
		static void SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float texIndex = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

		static void SubmitTriangle(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, float texIndex = 0.0f);

	private:
		// this is just checking uniform for debug
		static void Validate();

		static void Flush();
		static void Bind();

	private:
		struct Vertex
		{
			glm::vec4 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			Vertex()
				: Position(glm::vec4(0.0f)), Color(glm::vec4(0.0f)), TexCoord(glm::vec2(0.0f)), TexIndex(0.0f) {}

			Vertex(const glm::vec4& position, const glm::vec4& color, const glm::vec2& texCoord, float texIndex)
				: Position(position), Color(color), TexCoord(texCoord), TexIndex(texIndex) {}
		};

		struct BufferData
		{
			static constexpr uint64_t maxVertex = 1024;
			static constexpr uint64_t maxIndexBuffer = maxVertex * 6;

			// this is need to change with array, but not std::array
			std::vector<Vertex> vertexData; // size =  max vertex
			std::vector<uint32_t> indicesData; // size = max Index Buffer

			std::vector<std::shared_ptr<Texture2D>>TextureContainer; // size = 32
			std::vector<uint32_t> TextureSlotContainer; // size = 32

			static uint32_t vertexIndex;
			static uint32_t indicesIndex;
			static uint32_t indicesOffset;
		};

		struct Meshes
		{
			Material m_Material;

			std::shared_ptr<Shader> m_Shader;
			std::shared_ptr<VertexArray> m_VertexArray;

			// TEMPORARY 
			std::shared_ptr<VertexBuffer> vertexBuffer;
			std::shared_ptr<IndexBuffer> indexBuffer;
		};

	private:
		static BufferData* s_Buffer;
		static Meshes* s_Meshes;
	};

}