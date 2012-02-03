
local action = _ACTION or ""

solution "Riot"
    location(action);
    configurations { "Debug", "Release" }
    platforms { "x64" }
    targetdir("../build/bin");
    flags{ "Symbols", "NoMinimalRebuild", "ExtraWarnings", "NoExceptions", "NoPCH", "NoRTTI" }
    configuration "windows"
        buildoptions { "/MP" }
    
    project "RiotLib"
        kind        "StaticLib"
        language    "C++"
        files { "../lib/**.*" }
        includedirs { "../include" }
        objdir { "../build/obj/lib" }
        
        configuration "not macosx"
            excludes { "../**.m", "../**_macosx**"}
        configuration "not windows"
            excludes { "../**_win32**" }
        configuration "windows"
            buildoptions { "/MP" }