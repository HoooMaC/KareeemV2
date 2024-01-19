#include "Core/Kareeem.h"
#include "FrameBuffer.h"

#include "Renderer/BaseRendererAPI.h"

#include "API/OpenGL/OpenGLFrameBuffer.h"

namespace Karem {

	std::shared_ptr<FrameBuffer> CreateFrameBuffer(FrameBufferSpecifications specifications)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OpenGL: return std::make_shared<OpenGLFrameBuffer>(specifications);
		}
		ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet");
		return nullptr;
	}

}