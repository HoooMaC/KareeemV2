#pragma once

#include <utility>

namespace Karem {

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetTextureColorAttachmentID() const = 0;
		virtual const std::pair<int32_t, int32_t>& GetFrameBufferSize() const = 0;
			
		virtual void Resize(int32_t width, int32_t height) = 0;
		virtual void Invalidate() = 0;
	};

	std::shared_ptr<FrameBuffer> CreateFrameBuffer(int32_t width, int32_t height);

}