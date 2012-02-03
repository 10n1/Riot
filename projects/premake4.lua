
local action = _ACTION or ""

solution "Riot"
    location(action);
    configurations { "Debug", "Release" }
    platforms { "x64" }
    targetdir("../build/bin");
    flags{ "Symbols", "NoMinimalRebuild", "ExtraWarnings", "NoExceptions", "NoPCH", "NoRTTI" }
    
    configuration "windows"
        buildoptions { "/MP" }

    project "RiotGame"
        kind        "ConsoleApp"
        language    "C++"
        files { "../game/**.*" }
        includedirs { "../include" }
        objdir ( "../build/obj/game" )
        links { "RiotLib", "RiotTest", "Box2D", "lua" }
        
        configuration { "windows" }
            postbuildcommands { "$(TargetDir)RiotTest.exe" }
        
        configuration { "windows", "Debug" }
            libdirs { "../external/Box2D_v2.2.1/Build/vs2010/bin/Debug", "../external/lua-5.2.0/Build/vs2010/bin/Debug" }
        configuration { "windows", "Release" }
            libdirs { "../external/Box2D_v2.2.1/Build/vs2010/bin/Release", "../external/lua-5.2.0/Build/vs2010/bin/Release" }
            
    project "RiotTest"
        kind        "ConsoleApp"
        language    "C++"
        files { "../test/**.*" }
        includedirs { "../include" }
        objdir ( "../build/obj/test" )
        links { "RiotLib" }
        
    project "RiotLib"
        kind        "StaticLib"
        language    "C++"
        files { "../lib/**.*" }
        includedirs { "../include", "../external/Box2D_v2.2.1", "../external/lua-5.2.0/src" }
        objdir ( "../build/obj/lib" )
        links { "Box2D", "lua" }

        configuration "not macosx"
            excludes { "../**.m", "../**macosx**"}
        configuration "not windows"
            excludes { "../**win32**" }
        
        configuration { "windows", "Debug" }
            libdirs { "../external/Box2D_v2.2.1/Build/vs2010/bin/Debug", "../external/lua-5.2.0/Build/vs2010/bin/Debug" }
        configuration { "windows", "Release" }
            libdirs { "../external/Box2D_v2.2.1/Build/vs2010/bin/Release", "../external/lua-5.2.0/Build/vs2010/bin/Release" }
            
            