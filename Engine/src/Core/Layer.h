#pragma once

#include "Core/Kareeem.h"

namespace Karem {

	class Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGUIRender() {}
	protected:
		std::string m_DebugName;
	};

}