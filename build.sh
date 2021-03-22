#!/bin/sh

set -xe

CXX="${CXX:-g++}"
PKGS="sdl2 glew"
CXXFLAGS="-Wall -Wextra -std=c++17 -pedantic -fno-exceptions" -D_REENTRANT

$CXX $CXXFLAGS `pkg-config --cflags $PKGS` -o something.debug src/something.cpp `pkg-config --libs $PKGS`
