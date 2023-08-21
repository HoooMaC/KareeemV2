#pragma once

#include "Core/Kareeem.h"
#include "Core/Layer.h"

namespace Karem {

	class Layers
	{
	public:
		Layers();
		~Layers();

		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void PopLayer(std::shared_ptr<Layer> layer);
		void PopOverlay(std::shared_ptr<Layer> layer);

		std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<std::shared_ptr<Layer>> m_Layers;
		unsigned int m_InsertIndex = 0;
	};

}
