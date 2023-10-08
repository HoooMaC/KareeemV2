#pragma once

#include <utility>

namespace Karem {

	class FrameBuffer
	{
	protected:
		struct FrameBufferSize
		{
			int32_t width, height;
		};
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetTextureColorAttachmentID() const = 0;
		virtual FrameBufferSize GetFrameBufferSize() const = 0;
			
		virtual void Resize(int32_t width, int32_t height) = 0;
		virtual void Invalidate() = 0;
	};

	std::shared_ptr<FrameBuffer> CreateFrameBuffer(int32_t width, int32_t height);

}