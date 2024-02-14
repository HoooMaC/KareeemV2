#pragma once

#include "Renderer/BaseBuffer.h"
#include "Renderer/BaseTexture.h"
#include "Renderer/SubTexture.h"
#include "Renderer/Material.h"

#include <Rahman/SmartRef.h>

#include <glm/glm.hpp>

namespace Karem {

	class EditorCamera;

	class Renderer
	{
	public:
		static void Initialize();
		static void Shutdown();

		/*Material material*/
		static void BeginScene(EditorCamera& camera);
		static void BeginScene(const glm::mat4& projection, const glm::mat4& cameraTransform);
		static void EndScene();

		static void SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, int EntityId, int texIndex = 0);
		static void SubmitQuad(const glm::vec4& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, int entityId, int texIndex = 1, const glm::vec4& color = glm::vec4(1.0f));

		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color, int entityId, int texIndex = 0);
		static void SubmitQuad(const glm::mat4& transform, const Rahman::SmartRef<Texture2D> texture, int entityId, int texIndex = 1, const glm::vec4& color = glm::vec4(1.0f));
		static void SubmitQuad(const glm::mat4& transform, const SubTexture2D& subTexture, int entityId, int texIndex = 1, const glm::vec4& color = glm::vec4(1.0f));

		static void SubmitSubTexturedQuad(const glm::vec4& pos, const glm::vec2& size, const SubTexture2D& subTexture, int entityId, int texIndex = 1, const glm::vec4& color = glm::vec4(1.0f));

		static void SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, const glm::vec4& color, int entityId, int texIndex = 0);
		static void SubmitRotatedQuad(const glm::vec4& pos, const glm::vec2& size, float rotation, Rahman::SmartRef<Texture2D> texture, int entityId, int texIndex = 1, const glm::vec4& color = glm::vec4(1.0f));

		static void SubmitTriangle(const glm::vec4& pos, const glm::vec2& size, const glm::vec4& color, int entityId, int texIndex = 0);

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
			int TexIndex;
			int EntityId;

			Vertex()
				: Position(glm::vec4(0.0f)), Color(glm::vec4(0.0f)), TexCoord(glm::vec2(0.0f)), TexIndex(0), EntityId(-1) {}

			Vertex(const glm::vec4& position, const glm::vec4& color, const glm::vec2& texCoord, int texIndex, int entityId)
				: Position(position), Color(color), TexCoord(texCoord), TexIndex(texIndex), EntityId(entityId) {}
		};

		struct BufferData
		{
			static constexpr uint64_t maxVertex = 1024;
			static constexpr uint64_t maxIndexBuffer = maxVertex * 6;

			// this is need to change with array, but not std::array
			std::vector<Vertex> vertexData; // size =  max vertex
			std::vector<uint32_t> indicesData; // size = max Index Buffer

			std::vector<Rahman::SmartRef<Texture2D>> TextureContainer; // size = 32
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