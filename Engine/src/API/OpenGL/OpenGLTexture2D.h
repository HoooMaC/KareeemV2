#pragma once

#include <string>

#include "Renderer/BaseTexure.h"

namespace Karem {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filePath, uint32_t slot);
		~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetSlot() const override { return m_Slot; }
		std::string GetFilePath() const override { return m_FilePath; }

		void SetSlot(uint32_t slot) override { m_Slot = slot; }

		void Bind() const override;
		void UnBind() const override;
	private:
		void CreateTexture();
		void ClearTexture();

	private:
		std::string m_FilePath;
		uint32_t m_Slot;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};

}