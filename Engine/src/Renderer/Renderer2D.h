#pragma once

#include <string>

#include <glm/glm.hpp>

//#include "Renderer/OrthographicCamera.h"

namespace Karem {

	class OrthographicCamera;
	class Texture2D;

	class Renderer2D
	{
	public:
		static void Initialize();
		static void Shutdown();

		/*Material material*/
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float texIndex = 0.0f);
		static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float texIndex = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void SubmitRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color, float texIndex = 0.0f);
		static void SubmitRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float texIndex = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

		static void SubmitTriangle(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float texIndex = 0.0f);

	private:
		// this is just checking uniform for debug
		static void Validate();

		static void Bind();
		static void Flush();
	};

}
