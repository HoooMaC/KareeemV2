#pragma once

#include "Renderer/BaseBuffer.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/BaseTexture.h"
#include "Renderer/SubTexture.h"

#include "Renderer/RendererData/RendererData.h"

#include <glm/glm.hpp>

namespace Karem {

	class Renderer
	{
	public:
		static void Initialize();
		static void Shutdown();

		/*Material material*/
		static void BeginScene(const glm::mat4& viewProjection);
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
		static RendererData* s_Buffer;
		static RendererMeshes* s_Meshes;
	};

}