SET CXX=g++
SET PKGS=sdl2 glew --static
SET CXXFLAGS=-Wall -Wextra -std=c++17 -pedantic -fno-exceptions -D_GNU_SOURCE

rem inspired  https://stackoverflow.com/questions/2323292/assign-output-of-a-program-to-a-variable-using-a-ms-batch-file
pkg-config --cflags %PKGS% >temp.txt
set /p PKGSCFLAGS=<temp.txt
pkg-config --libs %PKGS% >temp.txt
set /p PKGLIBS=<temp.txt
del temp.txt

%CXX% %CXXFLAGS% -o config_indexer.exe src/config_indexer.cpp src/config_parser.cpp

setlocal
for %%i in (%*) do (
    if "%%i" == "--release" (
        set SOMETHING_RELEASE=1
    ) else (
        echo ERROR: Unknown flag "%%i"
        goto :eof
    )
)

if defined SOMETHING_RELEASE (
    config_indexer --bake assets/vars.conf >src/config_index.hpp
    %CXX% %CXXFLAGS% -O3 %PKGSCFLAGS% -DSOMETHING_RELEASE -o something.release.exe src/something.cpp %PKGLIBS% -lopengl32 -static
) else (
    config_indexer assets/vars.conf >src/config_index.hpp
    %CXX% %CXXFLAGS% %PKGSCFLAGS% -o something.debug.exe src/something.cpp %PKGLIBS% -lopengl32 -static
)
dir *.exe
REM Returns Errorcode. Zero is Noerror.
EXIT /B %ERRORLEVEL%
endlocal

