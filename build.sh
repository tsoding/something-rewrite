#!/bin/sh

set -xe

CXX="${CXX:-g++}"
PKGS="sdl2 glew"
CXXFLAGS="-Wall -Wextra -Wswitch-enum -std=c++17 -pedantic -fno-exceptions -ggdb"

if [ `uname` = "Darwin" ]; then
	CXXFLAGS+=" -framework OpenGL"
fi

while [ $# -gt 0 ]; do
    case $1 in
        --release)
            export SOMETHING_RELEASE="1"
            ;;
        *)
            echo "ERROR: Unknown flag $1"
            exit 1
            ;;
    esac

    shift
done

$CXX $CXXFLAGS -o config_indexer src/config_indexer.cpp src/config_parser.cpp

if [ "$SOMETHING_RELEASE" ]; then
    ./config_indexer --bake ./assets/vars.conf > ./src/config_index.hpp
    $CXX $CXXFLAGS -O3 `pkg-config --cflags $PKGS` -DSOMETHING_RELEASE -o something.release src/something.cpp `pkg-config --libs $PKGS`
else
    ./config_indexer ./assets/vars.conf > ./src/config_index.hpp
    $CXX $CXXFLAGS `pkg-config --cflags $PKGS` -o something.debug src/something.cpp `pkg-config --libs $PKGS`
fi
