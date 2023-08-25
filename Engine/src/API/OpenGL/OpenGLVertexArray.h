#pragma once

#include "Core/Renderer/BufferBase.h"

namespace Karem {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Init();
		void Clear();

		void Bind() const;
		void UnBind() const;
	private:
		uint32_t m_RendererID;
	};

}
