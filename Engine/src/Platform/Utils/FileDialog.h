#pragma once

#include <string>

namespace Karem {

	class FileDialog
	{
	public:
		static [[nodiscard]] std::string OpenFile(const char* filter);
		static [[nodiscard]] std::string SaveFile(const char* filter);
	};
	
}
