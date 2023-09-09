#include "Core/Kareeem.h"
#include "Renderer2D.h"

#include "Renderer/Material.h"
#include "Renderer/BaseShader.h"
#include "Renderer/BaseBuffer.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/RenderCommand.h"

namespace Karem {

	struct Renderer2DData
	{
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Material> m_Material;
	};

	static Renderer2DData* s_Data;

	void Renderer2D::Initialize()
	{
		s_Data = new Renderer2DData;
		s_Data->m_Shader = CreateShader("res\\shader\\position_color_vertex.glsl", "res\\shader\\position_color_fragment.glsl");
		s_Data->m_VertexArray = CreateVertexArray();


		// We need a vertex and index buffer

		BufferLayout layout;
		layout = GetShaderAttributes();
		GetShaderUniforms();
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene()
	{
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::SubmitQuade(const std::string& objId, const glm::vec3& pos, const glm::vec2 size, const glm::vec4& color)
	{
	}

	void Renderer2D::GenerateQuad(const glm::vec3& pos, const glm::vec2 size, const glm::vec4& color)
	{
	}

	BufferLayout Renderer2D::GetShaderAttributes()
	{
		return RendererCommand::GetShaderAttributes(s_Data->m_Shader);
	}

	void Renderer2D::GetShaderUniforms()
	{
		return RendererCommand::GetShaderUniforms(s_Data->m_Shader);
	}

	void Renderer2D::Validate()
	{
	}

	void Renderer2D::Bind()
	{
	}

	void Renderer2D::Flush()
	{
	}

}