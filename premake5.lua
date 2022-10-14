workspace "QPAllocators"
    architecture "x64"
    startproject "TestProject"
    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "QPA"
include "TestProject"