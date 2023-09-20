#pragma once

#include <memory>
#include <string>

#include "Renderer/TextureFormat.h"

namespace Karem {

	struct TextureData
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Slot;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetSlot() const = 0;
		virtual uint32_t GetTextureID() const = 0;
		virtual std::string GetFilePath() const = 0;

		virtual void SetSlot(uint32_t slot) = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};

	class Texture2D : public Texture
	{
	};

	std::shared_ptr<Texture2D> CreateTexture2D(uint32_t slot = 0);
	std::shared_ptr<Texture2D> CreateTexture2D(const std::string& filePath, uint32_t slot = 0, uint32_t filter = KAREM_NEAREST);

}