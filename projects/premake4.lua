
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
        links { "RiotLib", "Box2D", "lua" }
        
	configuration { "not xcode4" }
            links { "RiotTest" }
 
        configuration { "windows", "Debug" }
            libdirs { "../external/Box2D_v2.2.1/Build/vs2010/bin/Debug", "../external/lua-5.2.0/Build/vs2010/bin/Debug" }
        configuration { "windows", "Release" }
            libdirs { "../external/Box2D_v2.2.1/Build/vs2010/bin/Release", "../external/lua-5.2.0/Build/vs2010/bin/Release" }
            
        configuration { "macosx", "Debug" }
            libdirs { "../external/Box2D_v2.2.1/Build/gmake/bin/Debug", "../external/lua-5.2.0/Build/gmake/bin/Debug" }
        configuration { "macosx", "Release" }
            libdirs { "../external/Box2D_v2.2.1/Build/gmake/bin/Release", "../external/lua-5.2.0/Build/gmake/bin/Release" }
            
    project "RiotTest"
        kind        "ConsoleApp"
        language    "C++"
        files { "../test/**.*" }
        includedirs { "../include", "../external/UnitTest++/src" }
        objdir ( "../build/obj/test" )
        links { "RiotLib", "UnitTest++" }
        
        configurations { "Debug", "Release" }
        
        configuration { "windows" }
            postbuildcommands { "$(TargetDir)RiotTest.exe" }
       
        configuration { "gmake" }
            postbuildcommands { "../../build/bin/RiotTest" }
        
        configuration { "Debug" }
            libdirs { "../external/UnitTest++/Debug" }
        configuration { "Release" }
            libdirs { "../external/UnitTest++/Release" }
        
    project "RiotLib"
        kind        "StaticLib"
        language    "C++"
        files { "../include/**.*", "../lib/**.*" }
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
            
            
