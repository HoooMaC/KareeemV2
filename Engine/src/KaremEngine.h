#pragma once

//All the Library is Here
#include "Core/Kareeem.h"

#include "Core/TimeStep.h"
#include "Core/Application.h"
#include "Env/Color.h"

#include "Event/Event.h"
#include "Event/Input.h"
#include "Event/AppEvent.h"
#include "Event/KeyEvent.h"
#include "Event/KeyCode.h"
#include "Event/MouseEvent.h"

#include "Layer/Layer.h"
#include "Layer/Layers.h"
#include "Layer/ImGUILayer.h"

#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

#include "Renderer/Renderer.h"
#include "Renderer/BaseShader.h"
#include "Renderer/BaseBuffer.h"
#include "Renderer/SubTexture.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/BaseTexture.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/TextureFormat.h"
#include "Renderer/BaseRendererAPI.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/PerspectiveCamera.h"
#include "Renderer/OrthographicCamera.h"

// THIS IS SPECIFIC TO WINDOW
// #define KAREM_WINDOWS
#include "Platform/Windows/Platform.h"
