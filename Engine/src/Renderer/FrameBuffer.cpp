#include "Core/Kareeem.h"
#include "FrameBuffer.h"

#include "Renderer/BaseRendererAPI.h"

#include "API/OpenGL/OpenGLFrameBuffer.h"

namespace Karem {

	std::shared_ptr<FrameBuffer> CreateFrameBuffer(int32_t width, int32_t height)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OpenGL: return std::make_shared<OpenGLFrameBuffer>(width, height);
		}
		ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet");
		return nullptr;
	}

}