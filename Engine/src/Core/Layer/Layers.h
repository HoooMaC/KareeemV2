#pragma once

#include "Core/Kareeem.h"
#include "Core/Layer/Layer.h"

namespace Karem {

	class Layers
	{
	public:
		Layers();
		virtual ~Layers();

		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void PopLayer(std::shared_ptr<Layer> layer);
		void PopOverlay(std::shared_ptr<Layer> layer);

		inline size_t GetSize() const { return m_Layers.size(); }
		inline std::shared_ptr<Layer>& GetLayerAt(int ind) { return m_Layers[ind]; }

		std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end() { return m_Layers.end(); }

		std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }
	private:
		std::vector<std::shared_ptr<Layer>> m_Layers;
		unsigned int m_InsertIndex = 0;
	};

}
