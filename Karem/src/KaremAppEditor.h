#pragma once

#include "KaremEditorLayer.h"

namespace Karem {

	class KaremEditor final : public Application
	{
	public:
		KaremEditor();
		~KaremEditor() = default;

	private:
		std::shared_ptr<Layer> layer;
	};

}