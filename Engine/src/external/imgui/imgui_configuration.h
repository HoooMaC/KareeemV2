#pragma once

#include <filesystem>

struct ImFont;
struct ImGuiWindow;

namespace Karem {

    void InitializeImGui();
    void ShutdownImGui();

    void BeginFrame();
    void EndFrame();
    
    bool IsImGuiContextValid();

    ImFont* AddFontToLibrary(std::filesystem::path fontPath, float fontSize);
}
