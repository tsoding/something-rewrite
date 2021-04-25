#!/bin/sh

set -xe

CXX="${CXX:-g++}"
PKGS="sdl2 glew"
CXXFLAGS="-Wall -Wextra -std=c++17 -pedantic -fno-exceptions -ggdb"

if [ `uname` == "Darwin" ]; then
	CXXFLAGS+=" -framework OpenGL"
fi

$CXX $CXXFLAGS `pkg-config --cflags $PKGS` -o something.debug src/something.cpp `pkg-config --libs $PKGS`
