#pragma once

#include "KaremEngine.h"

class TriangleLayer : public Karem::Layer
{
public:
	TriangleLayer(const std::string& layerName = "Triangle") : Layer(layerName) {}

	void OnAttach() override;
	void OnUpdate(Karem::TimeStep ts) override;

	// this is temporary, just for experiment
	void setProjectionView(const glm::mat4& matrix) { m_ProjectionViewMatrix = matrix; }
private:
	std::shared_ptr<Karem::Shader> m_TriangleShader;
	std::shared_ptr<Karem::VertexArray> m_TriangleVertexArray;

	glm::mat4 m_ProjectionViewMatrix;
};
