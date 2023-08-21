project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	systemversion "latest"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "-%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "-%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}
	
	defines 
	{ 
	}

	filter "configurations:Logging"
		defines { "_DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Debug"
		defines { "_DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines { "NDEBUG" }
		runtime "Release"
		optimize "on"
