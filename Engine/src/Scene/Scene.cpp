#include "Core/Kareeem.h"
#include "Scene.h"

#include "Core/CoreFunction.h"

#include "Scene/Components.h"

#include "Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Karem{

	Scene::Scene()
	{
		m_SquareEntity = CreateEntity("First Square With entt");
		m_Registry.emplace<ColorComponent>(m_SquareEntity, HexToVec4("#0000ff"));
	}

	entt::entity Scene::CreateEntity(const std::string& entityName)
	{
		entt::entity newEntity =  m_Registry.create();
		m_Registry.emplace<TagComponent>(newEntity, entityName.c_str());
		m_Registry.emplace<TransformComponent>(newEntity, glm::mat4(1.0f));
		return newEntity;
	}

	void Scene::Update(TimeStep ts)
	{
		auto [transformOfSquare, color] = m_Registry.get<TransformComponent, ColorComponent>(m_SquareEntity);
		Karem::Renderer2D::SubmitQuad(transformOfSquare.Transform, color.Color);
	}

	void Scene::EventHandler(Event& e)
	{
	}

}