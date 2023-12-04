#pragma once

#include "Scene/Scene.h"

#include <vector>

namespace Karem {

	class SceneHierarcyPanel;

	class MenuBar
	{
		friend SceneHierarcyPanel;
	public:
		static void Render(std::shared_ptr<Scene>& scene, SceneHierarcyPanel& hierarcyPanel);

	private:
		static void NewScene(std::shared_ptr<Scene>& scene, SceneHierarcyPanel& hierarcyPanel);
		static void OpenScene(std::shared_ptr<Scene>& scene, SceneHierarcyPanel& hierarcyPanel);
		static void SaveSceneAs(std::shared_ptr<Scene>& scene, SceneHierarcyPanel& hierarcyPanel);

	private:
		static bool showEntityList;
		static bool showEntityComponent;
		static bool showCameraPanel;
	};

}
