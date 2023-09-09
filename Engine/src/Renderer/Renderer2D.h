#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Karem {

	class Renderer2D
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void BeginScene(/*Material material*/);
		static void EndScene();

		static void SubmitQuade(const std::string& objId, const glm::vec3& pos, const glm::vec2 size, const glm::vec4& color);
	private:
		static void GenerateQuad(const glm::vec3& pos, const glm::vec2 size, const glm::vec4& color);
		static BufferLayout GetShaderAttributes();
		static void GetShaderUniforms();

		// this is just checking uniform for debug
		static void Validate();

		static void Bind();
		static void Flush();
	};

}
