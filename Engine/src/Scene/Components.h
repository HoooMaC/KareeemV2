#pragma once

#include "Renderer/SubTexture.h"
#include "Renderer/CameraHandler.h"
#include <string>

#include <glm/glm.hpp>

namespace Karem {

	struct TagComponent
	{
		std::string Tag = "Undefined";

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const char* tag)
			: Tag(tag) {}	

	};

	struct TransformComponent
	{
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation), Rotation(0.0f), Scale(1.0f) {}

		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0,0,1 });

			return glm::translate(glm::mat4(1.0f), Translation) 
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct ColorComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);

		ColorComponent() = default;
		ColorComponent(const ColorComponent&) = default;
		ColorComponent(const glm::vec4& color)
			: Color(color) {}

		operator glm::vec4() { return Color; }
	};
	
	struct CameraComponent
	{
		CameraHandler Camera;
		//bool mainCamera = false;
		// TEMPORARY : maybe the default should be false
		bool mainCamera = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const OrthographicCamera& camera)
			: Camera(camera) {}
		CameraComponent(const PerspectiveCamera& camera)
			: Camera(camera) {}

		// TEMPORARY : Think for the better solution
		CameraComponent(const OrthographicCamera& camera, bool isMain)
			: Camera(camera), mainCamera(isMain) {}
		CameraComponent(const PerspectiveCamera& camera, bool isMain)
			: Camera(camera), mainCamera(isMain) {}

	};


}
