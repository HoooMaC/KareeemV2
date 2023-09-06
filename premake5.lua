workspace "Kareeem"
    architecture "x64"
    staticruntime "on"
    -- linkoptions { "/NODEFAULTLIB:library" }

    configurations 
    {
        "Logging",
        "Debug", 
        "Release"
    }

outputdir = "%{cfg.buildcfg}"

startproject "Client"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Vendor/GLFW/include"
IncludeDir["glad"] = "%{wks.location}/Vendor/glad/include"
IncludeDir["ImGUI"] = "%{wks.location}/Vendor/ImGUI"
IncludeDir["spdlog"] = "%{wks.location}/Vendor/spdlog/include"
IncludeDir["glm"] = "%{wks.location}/Vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Engine/src/external/stb_image"

project "Client"
    location "Client"
    language  "C++"
    cppdialect  "C++20"
    systemversion "latest"

    targetdir  ("%{wks.location}/bin/" .. outputdir.. "-%{prj.name}")
    objdir  ("%{wks.location}/bin-int/" .. outputdir .. "-%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }
    
    includedirs
    {
        "Engine/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.ImGUI}",
        "%{IncludeDir.glm}",
    }

    libdirs
    {
    }

    links
    {
        "Engine"
    }
    
    defines
    {
        "GLFW_INCLUDE_NONE"
    }
    
    filter "configurations:Logging"
        kind  "ConsoleApp"
        defines { "_DEBUG", "ACTIVATE_LOGGING" }
        includedirs { "%{IncludeDir.spdlog}" }
        runtime "Debug"

    filter "configurations:Debug"
        kind  "ConsoleApp"
        defines { "_DEBUG" }
        runtime "Debug"

    filter "configurations:Release"
        kind  "ConsoleApp"
        -- kind  "WindowedApp"
        defines { "NDEBUG" }
        runtime "Release"

project "Engine"
    location "Engine"
    language  "C++"
    kind  "StaticLib"
    cppdialect  "C++20"
    systemversion "latest"

    pchheader "Core/Kareeem.h"
	pchsource "%{prj.name}/src/Core/Kareeem.cpp"

    targetdir  ("bin/" .. outputdir.. "-%{prj.name}")
    objdir  ("bin-int/" .. outputdir .. "-%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    libdirs
    {
    }

    links 
    {
        "GLFW",
        "glad",
        "ImGUI",
    }
    
    includedirs
    {
        "Engine/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.ImGUI}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "configurations:Logging"
        defines { "_DEBUG", "ACTIVATE_LOGGING" }
        runtime "Debug"
        links { "spdlog" }
        includedirs {" %{IncludeDir.spdlog}" }
        
        filter "configurations:Debug"
        defines { "_DEBUG" }
        runtime "Debug"
        
        filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        
group "Dependencies"
        include "Vendor/GLFW"
        include "Vendor/ImGUI"
        include "Vendor/glad"
        include "Vendor/spdlog"
        include "Vendor/glm"