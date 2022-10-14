project "QPA"
location "."
kind "StaticLib"
language "C++"
cppdialect "C++17"
staticruntime "Off"

targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
objdir ("%{wks.location}/Temp/" .. outputdir .. "/%{prj.name}")

flags {
    "MultiProcessorCompile",
    "FatalWarnings"
}

files {
	"Source/**.h",
	"Source/**.cpp",
	"Source/**.hpp"
}

includedirs {
	"Source"
}

warnings "Extra"

filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "On"
    
filter{}