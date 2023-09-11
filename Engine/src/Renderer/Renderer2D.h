#pragma once

#include <string>

#include <glm/glm.hpp>

//#include "Renderer/OrthographicCamera.h"

namespace Karem {

	class OrthographicCamera;

	class Renderer2D
	{
	public:
		static void Initialize();
		static void Shutdown();

		/*Material material*/
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
	private:

		// this is just checking uniform for debug
		static void Validate();

		static void Bind();
		static void Flush();
	};

}
