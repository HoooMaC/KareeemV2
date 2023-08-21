#include "Core/Kareeem.h"
#include "Layers.h"


namespace Karem {

	Layers::Layers()
	{
	}

	Layers::~Layers()
	{
		for (auto layer : m_Layers)
			layer->OnDetach();
	}

	void Layers::PushLayer(std::shared_ptr<Layer> layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_InsertIndex, layer);
		m_InsertIndex++;
		layer->OnAttach();
	}

	void Layers::PushOverlay(std::shared_ptr<Layer> overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void Layers::PopLayer(std::shared_ptr<Layer> layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_InsertIndex, layer);
		if (it != m_Layers.begin() + m_InsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_InsertIndex--;
		}
	}

	void Layers::PopOverlay(std::shared_ptr<Layer> overlay)
	{
		auto it = std::find(m_Layers.begin() + m_InsertIndex, m_Layers.end(), overlay);
		if (it != m_Layers.begin() + m_InsertIndex)
		{
			overlay->OnDetach();
			m_Layers.erase(it);
			m_InsertIndex--;
		}

	}

}