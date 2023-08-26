#include "Core/Kareeem.h"

#include "BaseBuffer.h"

#include "API/OpenGL/OpenGLVertexArray.h"

namespace Karem {

	std::shared_ptr<VertexArray> VertexArray::CreateVertexArray()
	{
		return std::make_shared<OpenGLVertexArray>();
	}

}