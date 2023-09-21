#pragma once

//All the Library is Here
#include "Core/Kareeem.h"

#include "Core/TimeStep.h"
#include "Core/Application.h"
#include "Core/CoreFunction.h"

#include "Event/Event.h"
#include "Event/Input.h"
#include "Event/AppEvent.h"
#include "Event/KeyEvent.h"
#include "Event/KeyCode.h"
#include "Event/MouseEvent.h"

#include "Layer/Layer.h"
#include "Layer/Layers.h"
#include "Layer/ImGUILayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/BaseShader.h"
#include "Renderer/BaseBuffer.h"
#include "Renderer/BaseTexture.h"
#include "Renderer/SubTexture.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/TextureFormat.h"
#include "Renderer/BaseRendererAPI.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/OrthographicCamera.h"

#include "API/OpenGL/OpenGLShader.h"
#include "API/OpenGL/OpenGLIndexBuffer.h"
#include "API/OpenGL/OpenGLRendererAPI.h"
#include "API/OpenGL/OpenGLVertexArray.h"
#include "API/OpenGL/OpenGLVertexBuffer.h"
#include "API/OpenGL/OpenGLTexture2D.h"

// THIS IS SPECIFIC TO WINDOW
// #define KAREM_WINDOWS
#include "Platform/Windows/Platform.h"
