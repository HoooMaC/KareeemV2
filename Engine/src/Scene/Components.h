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
		glm::mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		//operator glm::mat4() { return Transform; }
		//operator glm::mat4() const { return Transform; }
	};

	struct ColorComponent
	{
		glm::vec4 Color;

		ColorComponent() = default;
		ColorComponent(const ColorComponent&) = default;
		ColorComponent(const glm::vec4& color)
			: Color(color) {}

		operator glm::vec4() { return Color; }
	};
	
	struct SubTextureComponent
	{
		SubTexture2D SubTexture;

		SubTextureComponent() = default;
		SubTextureComponent(const SubTextureComponent&) = default;
		SubTextureComponent(const std::shared_ptr<Texture2D>& reference, const glm::vec2& min, const glm::vec2& max)
			: SubTexture(reference, min, max) {}
	};

	struct CameraComponent
	{
		Karem::CameraHandler Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const OrthographicCamera& camera)
			: Camera(camera) {}
	};

}
