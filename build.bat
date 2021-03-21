SET CXX=g++
SET PKGS=sdl2 glew
SET CXXFLAGS=-Wall -Wextra -std=c++17 -pedantic -fno-exceptions
rem inspired  https://stackoverflow.com/questions/2323292/assign-output-of-a-program-to-a-variable-using-a-ms-batch-file
pkg-config --cflags %PKGS% >temp.txt
set /p PKGSCFLAGS=<temp.txt
pkg-config --libs %PKGS% >temp.txt
set /p PKGLIBS=<temp.txt
del temp.txt
rem more library added after inspired  https://stackoverflow.com/questions/51942342/glew-not-locating-opengl-functions-on-msys2
%CXX% %CXXFLAGS% %PKGSCFLAGS% -o something.debug.exe src/something.cpp %PKGLIBS% -lglfw3 -lglew32 -lglu32 -lopengl32
pause

