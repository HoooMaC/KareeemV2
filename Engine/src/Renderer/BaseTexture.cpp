#include "Core/Kareeem.h"

#include "BaseTexture.h"

#include "Renderer/BaseRendererAPI.h"

#include "API/OpenGL/OpenGLTexture2D.h"

namespace Karem {

	std::shared_ptr<Texture2D> Karem::CreateTexture2D(uint32_t slot)
	{
		switch (RendererAPI::GetAPI())
		{
		case API::None: ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet") break;
		case API::OpenGL: return std::make_shared<OpenGLTexture2D>(slot);
		}
		return nullptr;
	}

	std::shared_ptr<Texture2D> Karem::CreateTexture2D(std::string_view filePath, uint32_t slot, uint32_t filter)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::None: ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet") break;
			case API::OpenGL: return std::make_shared<OpenGLTexture2D>(filePath, slot, filter);
		}
		return nullptr;
	}

	Texture2D* Karem::CreateRawTexture2D(uint32_t slot)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::None: ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet") break;
			case API::OpenGL: return new OpenGLTexture2D(slot);
		}
		return nullptr;
	}

	bool Karem::DestroyRawTexture2D(Texture2D* texture)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::None: ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet") break;
			case API::OpenGL: delete texture;
		}
		return false;
	}

}