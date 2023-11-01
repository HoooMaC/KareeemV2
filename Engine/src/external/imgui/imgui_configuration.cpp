#include "Core/Kareeem.h"
#include "imgui_configuration.h"

#include "Env/Color.h"

#include "Renderer/GraphicsContext.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Karem {

	void InitializeImGui()
	{
		IMGUI_CHECKVERSION();
		ImGuiContext* ctx = ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		io.FontDefault = AddFontToLibrary("res/font/Helvetica.ttf", 14.0f);

		SetupKaremStyleDefault();
		KaremColorStyleDefault();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		auto* context = GraphicsContext::GetContextCurrent();
		ImGui_ImplGlfw_InitForOpenGL(context, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void ShutdownImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		auto* backup_current_context = GraphicsContext::GetContextCurrent();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		GraphicsContext::SetCurrentContext(backup_current_context);
	}

	void SetupKaremStyleDefault()
	{
#if 0
		float       Alpha;                      // Global alpha applies to everything in Dear ImGui.
		float       DisabledAlpha;              // Additional alpha multiplier applied by BeginDisabled(). Multiply over current value of Alpha.
		ImVec2      WindowPadding;              // Padding within a window.
		float       WindowRounding;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended.
		float       WindowBorderSize;           // Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		ImVec2      WindowMinSize;              // Minimum window size. This is a global setting. If you want to constrain individual windows, use SetNextWindowSizeConstraints().
		ImVec2      WindowTitleAlign;           // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
		ImGuiDir    WindowMenuButtonPosition;   // Side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to ImGuiDir_Left.
		float       ChildRounding;              // Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
		float       ChildBorderSize;            // Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		float       PopupRounding;              // Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
		float       PopupBorderSize;            // Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		ImVec2      FramePadding;               // Padding within a framed rectangle (used by most widgets).
		float       FrameRounding;              // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
		float       FrameBorderSize;            // Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
		ImVec2      ItemSpacing;                // Horizontal and vertical spacing between widgets/lines.
		ImVec2      ItemInnerSpacing;           // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
		ImVec2      CellPadding;                // Padding within a table cell
		ImVec2      TouchExtraPadding;          // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
		float       IndentSpacing;              // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
		float       ColumnsMinSpacing;          // Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1).
		float       ScrollbarSize;              // Width of the vertical scrollbar, Height of the horizontal scrollbar.
		float       ScrollbarRounding;          // Radius of grab corners for scrollbar.
		float       GrabMinSize;                // Minimum width/height of a grab box for slider/scrollbar.
		float       GrabRounding;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
		float       LogSliderDeadzone;          // The size in pixels of the dead-zone around zero on logarithmic sliders that cross zero.
		float       TabRounding;                // Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
		float       TabBorderSize;              // Thickness of border around tabs.
		float       TabMinWidthForCloseButton;  // Minimum width for close button to appear on an unselected tab when hovered. Set to 0.0f to always show when hovering, set to FLT_MAX to never show close button unless selected.
		ImGuiDir    ColorButtonPosition;        // Side of the color button in the ColorEdit4 widget (left/right). Defaults to ImGuiDir_Right.
		ImVec2      ButtonTextAlign;            // Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered).
		ImVec2      SelectableTextAlign;        // Alignment of selectable text. Defaults to (0.0f, 0.0f) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line.
		float       SeparatorTextBorderSize;    // Thickkness of border in SeparatorText()
		ImVec2      SeparatorTextAlign;         // Alignment of text within the separator. Defaults to (0.0f, 0.5f) (left aligned, center).
		ImVec2      SeparatorTextPadding;       // Horizontal offset of text from each edge of the separator + spacing on other axis. Generally small values. .y is recommended to be == FramePadding.y.
		ImVec2      DisplayWindowPadding;       // Window position are clamped to be visible within the display area or monitors by at least this amount. Only applies to regular windows.
		ImVec2      DisplaySafeAreaPadding;     // If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!
		float       MouseCursorScale;           // Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). We apply per-monitor DPI scaling over this scale. May be removed later.
		bool        AntiAliasedLines;           // Enable anti-aliased lines/borders. Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
		bool        AntiAliasedLinesUseTex;     // Enable anti-aliased lines/borders using textures where possible. Require backend to render with bilinear filtering (NOT point/nearest filtering). Latched at the beginning of the frame (copied to ImDrawList).
		bool        AntiAliasedFill;            // Enable anti-aliased edges around filled shapes (rounded rectangles, circles, etc.). Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).
		float       CurveTessellationTol;       // Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
		float       CircleTessellationMaxError; // Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.
		ImVec4      Colors[ImGuiCol_COUNT];
#endif
		auto& style = ImGui::GetStyle();
		style.WindowPadding = { 10.0f, 10.0f };
		style.FramePadding = { 4.f, 4.f };
		style.CellPadding = { 8.f, 2.f };
		style.ItemSpacing = { 10.0f, 4.f };
		style.ItemInnerSpacing = { 5.0f, 0.0f };
		style.TouchExtraPadding = { 0, 0 };
		style.IndentSpacing = 20.0f;
		style.ScrollbarSize = 13.0f;
		style.GrabMinSize = 20.0f;

		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = 0;
		style.TabBorderSize = 0;

		style.WindowRounding = 0;
		style.ChildRounding = 0;
		style.FrameRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 3.0f;
		style.GrabRounding = 3.0f;
		style.TabRounding = 3.0f;

		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ColorButtonPosition = ImGuiDir_Left;
		//style.ButtonTextAlign
		style.SelectableTextAlign = { 0.0f, 0.0f }; 
		style.SeparatorTextBorderSize = 3.0f;
		//style.SeparatorTextAlign
		style.SeparatorTextPadding = { 0.0f, 0.0f };
		//style.LogSliderDeadzone

		//style.DisplaySafeAreaPadding
	}

	void KaremColorStyleDefault()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		colors[ImGuiCol_Text]				= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled]		= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg]			= HexToVec4<ImVec4>(Color::WindowBackground);
		colors[ImGuiCol_ChildBg]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg]			= ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border]				= HexToVec4<ImVec4>(Color::Border);
		colors[ImGuiCol_BorderShadow]		= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg]			= HexToVec4<ImVec4>(Color::FrameBackground);
		colors[ImGuiCol_FrameBgHovered]		= HexToVec4<ImVec4>(Color::FrameBackgroundHovered);
		colors[ImGuiCol_FrameBgActive]		= HexToVec4<ImVec4>(Color::FrameBackgroundActive);
		colors[ImGuiCol_TitleBg]			= HexToVec4<ImVec4>(Color::TitleBackground);
		colors[ImGuiCol_TitleBgActive]		= HexToVec4<ImVec4>(Color::TitleBackgroundActive);
		colors[ImGuiCol_TitleBgCollapsed]	= HexToVec4<ImVec4>(Color::TitleBackgroundCollapsed);
		colors[ImGuiCol_MenuBarBg]			= HexToVec4<ImVec4>(Color::MenuBarBackground);
		colors[ImGuiCol_ScrollbarBg]		= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab]		= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]=ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_SliderGrab]			= HexToVec4<ImVec4>(Color::Slider); // !TEMPORARY : thinking about this again
		colors[ImGuiCol_SliderGrabActive]	= HexToVec4<ImVec4>(Color::Slider); // !TEMPORARY : thinking about this again
		colors[ImGuiCol_Button]				= HexToVec4<ImVec4>(Color::Button);
		colors[ImGuiCol_ButtonHovered]		= HexToVec4<ImVec4>(Color::ButtonHovered);
		colors[ImGuiCol_ButtonActive]		= HexToVec4<ImVec4>(Color::ButtonActive);
		colors[ImGuiCol_Header]				= HexToVec4<ImVec4>(Color::Header);
		colors[ImGuiCol_HeaderHovered]		= HexToVec4<ImVec4>(Color::HeaderHovered);
		colors[ImGuiCol_HeaderActive]		= HexToVec4<ImVec4>(Color::HeaderActive);
		colors[ImGuiCol_Separator]			= HexToVec4<ImVec4>(Color::Separator);
		colors[ImGuiCol_SeparatorHovered]	= HexToVec4<ImVec4>(Color::Separator);
		colors[ImGuiCol_SeparatorActive]	= HexToVec4<ImVec4>(Color::Separator);
		colors[ImGuiCol_ResizeGrip]			= ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered]	= ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive]	= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab]				= HexToVec4<ImVec4>(Color::Tab);
		colors[ImGuiCol_TabHovered]			= HexToVec4<ImVec4>(Color::TabHovered);
		colors[ImGuiCol_TabActive]			= HexToVec4<ImVec4>(Color::TabActive);
		colors[ImGuiCol_TabUnfocused]		= HexToVec4<ImVec4>(Color::TabUnfocused);
		colors[ImGuiCol_TabUnfocusedActive] = HexToVec4<ImVec4>(Color::TabUnfocusActive);
		colors[ImGuiCol_DockingPreview]		= colors[ImGuiCol_HeaderActive] /** ImVec4(1.0f, 1.0f, 1.0f, 0.7f)*/;
		colors[ImGuiCol_DockingEmptyBg]		= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines]			= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered]	= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram]		= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered]=ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg]		= ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong]	= ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableBorderLight]	= ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableRowBg]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt]		= ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg]		= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget]		= ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight]		= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight]=ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg]	= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg]	= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	bool IsImGuiContextValid()
	{
		return ImGui::GetCurrentContext() ? true : false;
	}


	ImFont* AddFontToLibrary(std::filesystem::path fontPath, float fontSize)
	{
		ImGuiIO& io = ImGui::GetIO();// (void)io;
		return io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), fontSize);
	}

}