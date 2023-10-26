#include "Core/Kareeem.h"
#include "imgui_configuration.h"

#include "Renderer/GraphicsContext.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Karem {

	void InitializeImGui()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGuiContext* ctx = ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();// (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		io.FontDefault = AddFontToLibrary("res/font/Helvetica.ttf", 14.0f);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
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