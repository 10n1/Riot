-- A solution contains projects, and defines the available configurations
solution "Riot"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    flags { "ExtraWarnings", "NoExceptions", "NoPCH", "NoRTTI" }
    
    configuration "Debug"
        defines { "DEBUG", "_DEBUG" }
        flags { "Symbols", "NoMinimalRebuild" }
        targetdir ("../build/bin/")
        objdir("../build/obj/")

    configuration "Release"
        defines { "NDEBUG" }
        flags { "OptimizeSpeed", "Symbols" }
        targetdir ("../build/bin/")
        objdir("../build/obj/")
        
    configuration "windows"
        defines { "WIN32" }
        buildoptions "/MP"
 
-------------------------------------------
-- Internal
-------------------------------------------   
    ---------------------------------------
    -- Game
    ---------------------------------------
    project "game"
        kind "ConsoleApp"
        language "C++"
        files { "../game/**.h", "../game/**.cpp", "../game/**.c", "../assets/**.*" }
        includedirs { "../systems", "../external/Box2D_v2.2.1/", "../external/lua-5.2.0/src" }
        location "../game"
        debugdir("../build/bin/")
        links { "graphicsDevice", "renderEngine", "systemInterface", "Box2D", "Lua" }
        objdir("../build/obj/"..project().name)

    configuration "windows"
        buildoptions "/MP"
        links { "d3d11", "d3dx11", "d3dcompiler", "dxguid" }
        
    configuration { "windows", "x64" }
        libdirs { "$(DXSDK_DIR)Lib/x64" }
        
    configuration { "windows", "x86" }
        libdirs { "$(DXSDK_DIR)Lib/x86" }

    configuration "macosx"
        links { "Cocoa.framework", "OpenGL.framework", "AppKit.framework", "Foundation.framework", "CoreData.framework" }
		
    ---------------------------------------
    -- Graphics device
    ---------------------------------------
    project "graphicsDevice"
        kind "StaticLib"
        language "C++"
        files { "../systems/graphicsDevice/**.c", "../systems/graphicsDevice/**.h" }
        includedirs { "../systems", "../systems/graphicsDevice/glew" }
        location "../systems/graphicsDevice"
        objdir("../build/obj/"..project().name)

    configuration "windows"
        defines { "GLEW_STATIC" }
        includedirs { "$(DXSDK_DIR)include" }
        links { "d3d11", "d3dx11", "d3dcompiler" }
		excludes "../systems/graphicsDevice/**osx**"
        
    configuration { "windows", "x64" }
        libdirs { "$(DXSDK_DIR)Lib/x64" }
        
    configuration { "windows", "x86" }
        libdirs { "$(DXSDK_DIR)Lib/x86" }

    configuration "macosx"
        files { "../systems/graphicsDevice/**.m" }
		excludes { "../systems/graphicsDevice/**windows**", "../systems/graphicsDevice/**directx**" }
        links { "Cocoa.framework", "OpenGL.framework", "AppKit.framework", "Foundation.framework", "CoreData.framework" }
		buildoptions { "-pedantic" }
		
        
    ---------------------------------------
    -- Render Engine
    ---------------------------------------
    project "renderEngine"
        kind "StaticLib"
        language "C++"
        files { "../systems/renderEngine/**.cpp", "../systems/renderEngine/**.h" }
        includedirs { "../systems" }
        location "../systems/renderEngine"
        links { "graphicsDevice" }
        objdir("../build/obj/"..project().name)
 
    configuration "windows"
        links { "d3d11", "d3dx11", "d3dcompiler" }
        buildoptions "/MP"

    configuration "macosx"
        links { "Cocoa.framework", "OpenGL.framework", "AppKit.framework", "Foundation.framework", "CoreData.framework" }
		buildoptions { "-pedantic" }
        
    ---------------------------------------
    -- system
    ---------------------------------------
    project "systemInterface"
        kind "StaticLib"
        language "C++"
        files { "../systems/system/**.c", "../systems/system/**.h" }
        includedirs { "../systems" }
        location "../systems/system"
        objdir("../build/obj/"..project().name)
 
    configuration "windows"
        links { "d3d11", "d3dx11", "d3dcompiler" }
        buildoptions "/MP"
		excludes "../systems/system/**osx**"

    configuration "macosx"
        files { "../systems/system/**.m" }
		excludes { "../systems/system/**win32**" }
        links { "Cocoa.framework", "OpenGL.framework", "AppKit.framework", "Foundation.framework", "CoreData.framework" }
		buildoptions { "-pedantic" }


-------------------------------------------
-- External
-------------------------------------------
    local action = _ACTION or ""
    local box2ddir = "../external/Box2D_v2.2.1/"
    
	project "Box2D"
		kind "StaticLib"
		language "C++"
        location ( box2ddir.."Build/" .. action )
		files { box2ddir.."Box2D/**.h", box2ddir.."Box2D/**.cpp" }
		includedirs { box2ddir }
        objdir("../build/obj/"..project().name)
        
        
    local luadir = "../external/lua-5.2.0/"
    
	project "Lua"
		kind "StaticLib"
		language "C"
        location ( luadir.."build/" )
		files { luadir.."src/**.h", luadir.."src/**.c" }
		includedirs { luadir.."src/" }
        excludes { luadir.."src/lua.c", luadir.."src/luac.c" }
        objdir("../build/obj/"..project().name)