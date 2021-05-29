@echo off
rem launch this from msvs-enabled console

set CXXFLAGS=/std:c++17 /FC /Zi /W4 /WX /wd4458 /wd4996 /nologo
set INCLUDES=/I dependencies\SDL2\include /I dependencies\GLFW\include /I dependencies\GLEW\include
set LIBS=dependencies\SDL2\lib\x64\SDL2.lib ^
         dependencies\SDL2\lib\x64\SDL2main.lib ^
         dependencies\GLFW\lib\glfw3.lib ^
         dependencies\GLEW\lib\glew32s.lib ^
         opengl32.lib User32.lib Gdi32.lib Shell32.lib

for %%i in (%*) do (
    if "%%i" == "--release" (
        set SOMETHING_RELEASE=1
    ) else (
        echo ERROR: Unknown flag "%%i"
        goto :eof
    )
)

cl.exe %CXXFLAGS% /Fe"config_indexer.exe" src/config_indexer.cpp src/config_parser.cpp

if defined SOMETHING_RELEASE (
    config_indexer --bake assets/vars.conf >src/config_index.hpp
    cl.exe /O2 %CXXFLAGS% %INCLUDES% /Fe"something.release.exe" /DSOMETHING_RELEASE src/something.cpp /link %LIBS% -SUBSYSTEM:windows
) else (
    config_indexer assets/vars.conf >src/config_index.hpp
    cl.exe /Od %CXXFLAGS% %INCLUDES% /Fe"something.debug.exe" src/something.cpp /link %LIBS% -SUBSYSTEM:windows
)

rem To remove enviroment variable from current cmd session
set SOMETHING_RELEASE=
