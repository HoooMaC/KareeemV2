#include "Core/Kareeem.h"

#include "BaseBuffer.h"

#include "Renderer/BaseRendererAPI.h"

#include "API/OpenGL/OpenGLVertexArray.h"
#include "API/OpenGL/OpenGLVertexBuffer.h"
#include "API/OpenGL/OpenGLIndexBuffer.h"

namespace Karem {

	std::shared_ptr<VertexArray> CreateVertexArray()
	{
		switch (RendererAPI::GetAPI())
		{
			case API::None: ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet") break;
			case API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> CreateVertexBuffer(void* data, size_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::None: ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet") break;
			case API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(data, size);
		}
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> CreateIndexBuffer(void* data, uint64_t count)
	{
		
		switch (RendererAPI::GetAPI())
		{
			case API::None: ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet") break;
			case API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(data, count);
		}
		return nullptr;
	}

}