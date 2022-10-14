project "TestProject"
location "."
kind "ConsoleApp"
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
	"**.h",
	"**.cpp",
	"**.hpp"
}

includedirs{
    "%{wks.location}/QPA/Source"
}

links {
    "QPA"
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