@echo off
rem launch this from msvs-enabled console

set CXXFLAGS=/std:c++17 /O2 /FC /Zi /W4 /WX /wd4458 /wd4996 /nologo
set INCLUDES=/I dependencies\SDL2\include /I dependencies\GLFW\include /I dependencies\GLEW\include
set LIBS=dependencies\SDL2\lib\x64\SDL2.lib ^
         dependencies\SDL2\lib\x64\SDL2main.lib ^
         dependencies\GLFW\lib\glfw3.lib ^
         dependencies\GLEW\lib\glew32s.lib ^
         opengl32.lib User32.lib Gdi32.lib Shell32.lib

cl.exe %CXXFLAGS% /Fe"config_indexer.exe" src/config_indexer.cpp
config_indexer assets/vars.conf >src/config_index.hpp

cl.exe %CXXFLAGS% %INCLUDES% /Fe"something.debug.exe" src/something.cpp ^
    /link %LIBS% -SUBSYSTEM:windows

rem TODO(#74): no release build on CI for msvc
