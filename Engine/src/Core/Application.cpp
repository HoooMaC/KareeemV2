#include "Core/Kareeem.h"

#include "Application.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

#include "Platform/Windows/Platform.h"

//#include <KaremAppEditor.h>
#include "../../Karem/src/KaremAppEditor.h"

#include <imgui.h>
#include "external/imgui/imgui_configuration.h"

namespace Karem {

    inline constinit Application* s_Application = nullptr;

    // TEMPORARY -- this is should in Application.cpp
    void StartUpApplication(AppType type)
    {
        switch (type)
        {
            case Karem::AppType::None:
                break;
            case Karem::AppType::GameEngine:
                s_Application = new KaremEditor();
                break;
        }
    }


    void RunApplication()
    {
        s_Application->Run();
    }

    void ShutdownApplication()
    {
        delete s_Application;
    }

    Application::Application(const WindowProperty& props) : m_Window(props)
    {
        m_Window.SetEventCallbacks(std::bind(&Application::EventHandler, this, std::placeholders::_1));
        InitializeImGui();
        
        Renderer::Initialize();
    }

    Application::~Application()
    {
        Renderer::Shutdown();
        ShutdownImGui();
        Shutdown();
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time = Platform::GetTime();
            TimeStep timeStep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (const std::shared_ptr<Layer>& layer : m_Layers)
            {
                if (layer->GetStatus())
                {
                    layer->Update(timeStep);
                    layer->RenderImGUI();
                }
            }
            m_Window.Update();
        }

    }

    void Application::EventHandler(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::WindowCloseAction, this, std::placeholders::_1));

        for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
        {
        	if((*it)->GetStatus())
        		(*it)->EventHandler(event);
        }

    }

    bool Application::WindowCloseAction(WindowCloseEvent& event)
    {
        m_Running = false;
        return true;
    }

}