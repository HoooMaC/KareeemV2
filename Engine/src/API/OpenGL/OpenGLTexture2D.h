#pragma once

#include <string>

#include "Renderer/BaseTexture.h"

namespace Karem {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t slot);
		OpenGLTexture2D(const std::string& filePath, uint32_t slot, uint32_t filter);
		~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetSlot() const override { return m_Slot; }
		uint32_t GetTextureID() const override { return m_RendererID; }
		std::string GetFilePath() const override { return m_FilePath; }

		void SetSlot(uint32_t slot) override { m_Slot = slot; }

		void Bind() const override;
		void UnBind() const override;
	private:
		void CreateTexture(uint32_t filter);
		void ClearTexture();

	private:
		std::string m_FilePath;
		uint32_t m_Slot;
		int32_t m_Width, m_Height, m_BPP;
		uint32_t m_RendererID;
	};

}