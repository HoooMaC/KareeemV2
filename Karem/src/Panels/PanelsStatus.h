#pragma once

#include <KaremEngine.h>

namespace Karem {

	enum PanelsStatus
	{
		EntityList = 0,
		EntityComponent,
		CameraPanel,

		Counts
	};

	inline const char* GetPanelString(PanelsStatus panel)
	{
		switch (panel)
		{
			case Karem::EntityList:
				return "Entity List";
			case Karem::EntityComponent:
				return "Entity Component";
			case Karem::CameraPanel:
				return "Camera Panel";
		}
		ENGINE_ASSERT(true, "Invalid");
		return "";
	}

}