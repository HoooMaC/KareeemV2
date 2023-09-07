#pragma once

#include "KaremEngine.h"

class AppLayer : public Karem::Layer
{
public:
	AppLayer(const std::string& layerName = "Application")
		: Layer(layerName) {}

	void OnAttach() override;
	void OnUpdate(Karem::TimeStep ts) override;

	void setProjectionView(const glm::mat4& matrix) { m_ProjectionViewMatrix = matrix; }
private:
	std::shared_ptr<Karem::Shader> m_SquareShader;
	std::shared_ptr<Karem::VertexArray> m_SquareVertexArray;
	std::shared_ptr<Karem::Texture2D> m_TextureBasic;

	glm::mat4 m_ProjectionViewMatrix;
	uint32_t m_TextureSlot;
};