#include "Core/Kareeem.h"

#include "OpenGLTexture2D.h"

#include <glad/glad.h>

namespace Karem {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath, uint32_t slot)
		: m_FilePath(filePath), m_Slot(slot), m_Width(0), m_Height(0), m_RendererID(0)
	{
		CreateTexture();
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ClearTexture();
	}

	void OpenGLTexture2D::Bind() const
	{
	}

	void OpenGLTexture2D::UnBind() const
	{
	}

	void OpenGLTexture2D::CreateTexture()
	{
	}

	void OpenGLTexture2D::ClearTexture()
	{
	}

}