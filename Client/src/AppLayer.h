#pragma once

#include "KaremEngine.h"

class AppLayer : public Karem::Layer
{
public:
	AppLayer(const std::string& layerName = "Application")
		: Layer(layerName) {}

	void OnAttach() override;
	void OnUpdate() override;

private:
	std::shared_ptr<Karem::VertexArray> m_VertexArraySquare;
	std::shared_ptr<Karem::Shader> m_ShaderSquare;
	glm::vec4 m_SquareColor;
	glm::mat4 m_ProjectionMatrix;

};