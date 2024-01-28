#pragma once

#include "PanelsStatus.h"

#include "Event/Event.h"
#include "Event/KeyEvent.h"

namespace Karem {

	class MenubarPanel
	{
		friend class Panels;
	public:
		MenubarPanel()
		{
			//static_assert((uint64_t)PanelsStatus::Counts * sizeof(bool) == sizeof(m_Panels), "Size mismatch");
			//memset(m_Panels, 0, sizeof(m_Panels));
		}

		void Render();
		void EventHandler(Event& event);

		bool CheckPanelStatus(PanelsStatus panel) const { ENGINE_ASSERT(panel == PanelsStatus::Counts, "Invalid panels"); return m_Panels[(int)panel]; }

		void DrawPanelMenuItem(PanelsStatus panel);
		bool* GetPanelPointer(PanelsStatus panel) { ENGINE_ASSERT(panel == PanelsStatus::Counts, "Invalid panels"); return &m_Panels[(int)panel]; }

	private:
		bool KeyPressedAction(KeyPressedEvent& event);

	private:
		std::function<void()> newSceneFunction;
		std::function<void(std::string_view)> saveSceneAsFunction;
		std::function<void(std::string_view)> openSceneFunction;

		bool m_Panels[(int)PanelsStatus::Counts];

	};

}