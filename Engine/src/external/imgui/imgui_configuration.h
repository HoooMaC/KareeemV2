#pragma once

#include <filesystem>

struct ImFont;

namespace Karem {

    void InitializeImGui();
    void ShutdownImGui();

    void BeginFrame();
    void EndFrame();
    
    ImFont* AddFontToLibrary(std::filesystem::path fontPath, float fontSize);
}
