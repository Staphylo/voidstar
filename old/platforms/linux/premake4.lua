function create_project( name )
	project( name .. "-app" )
		kind "WindowedApp"
		language "C++"
		files { "../../src/" .. name .. "/source/**.cpp", "platform_linux.cpp" }
		targetdir("../../src/" .. name .. "/")
		includedirs( "../../src/common/" )
		includedirs( "../../src/common/thirdparty/glm" )
		includedirs( "../../src/common/thirdparty/stb_image" )
		defines { "GLM_FORCE_RADIANS" }
		
		configuration "windows"
			links {"glu32", "opengl32", "gdi32", "winmm", "user32","GLEW"}

		configuration "linux"
			links {"GL","glfw","GLEW"}
		
		configuration "macosx"
			links {"GL","glfw","GLEW", "CoreFoundation.framework"}
			libdirs {"/opt/local/lib"}
			includedirs {"/opt/local/include"}
		
		configuration "haiku"
			links {"GL","glfw","GLEW"}

		configuration "freebsd"
			links {"GL","glfw","GLEW"}
		
		configuration "debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			buildoptions{ "-Wall" }
			targetname ( name .. "-debug" )

		configuration "release"
			defines { "NDEBUG" }
			flags { "Optimize" }
			buildoptions{ "-Wall" }
			targetname ( name .. "-release" )
end

solution "opengl-series"
	location("./")
	targetdir("./bin")
	configurations { "debug", "release" }
	objdir("obj/" .. os.get() .. "/")
	
	create_project( "miners-app" );
	--create_project( "01_project_skeleton" );
	--create_project( "02_textures" );
	--create_project( "03_matrices" );
	--create_project( "04_camera" );
	--create_project( "05_asset_instance" );
	--create_project( "06_diffuse_lighting" );
	--create_project( "07_more_lighting" );
	--create_project( "08_even_more_lighting" );