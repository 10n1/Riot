-- Lua script

local action = _ACTION or ""

solution "Lua"
	location ( "Build/" .. action )
	configurations { "Debug", "Release" }
	platforms { "x64" }
	
	configuration "Debug"
		targetdir ( "Build/" .. action .. "/bin/Debug" )
		flags { "Symbols" }

   configuration "Release"
		targetdir ( "Build/" .. action .. "/bin/Release" )
		defines { "NDEBUG" }
		flags { "Optimize" }

	project "Lua"
		kind "StaticLib"
		language "C"
		files { "src/**.h", "src/**.c" }
		excludes { "src/lua.c", "src/luac.c" }
		includedirs { "src" }
