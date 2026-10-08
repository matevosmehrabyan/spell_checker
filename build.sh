#!/usr/bin/bash

DIR="$(dirname "$(realpath "$0")")"
BUILD_DIR=$DIR/build
SPELL_CHECKER=$DIR/spell_checker

rm -rf $BUILD_DIR $SPELL_CHECKER
mkdir $BUILD_DIR

cmake -S "$DIR" -B "$DIR/build" -DCMAKE_INSTALL_PREFIX="$DIR"
cmake --build "$DIR/build"
cmake --install "$DIR/build"
