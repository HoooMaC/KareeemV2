workspace "Kareeem"
    architecture "x64"
    staticruntime "on"

    configurations 
    {
        "Debug", 
        "Release"
    }

outputdir = "%{cfg.buildcfg}"

startproject "Client"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Vendor/GLFW/include"

project "Client"
    location "Client"
    language  "C++"
    cppdialect  "C++20"
    systemversion "latest"

    targetdir  ("bin/" .. outputdir.. "-%{prj.name}")
    objdir  ("bin-int/" .. outputdir .. "-%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    links
    {
        "Engine",
    }

    includedirs
    {
        "Engine/src",
        "%{IncludeDir.GLFW}"
    }

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
        -- "opengl32.lib",
        "GLFW"
    }
    
    includedirs
    {
        "Engine/src",
        "%{IncludeDir.GLFW}"
    }

    filter "configurations:Debug"
        defines { "_DEBUG" }
        runtime "Debug"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"

group "Dependencies"
    include "Vendor/GLFW"