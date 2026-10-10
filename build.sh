#!/usr/bin/bash
set -e

BUILD_TYPE="${1:-Release}"

if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
    echo "Usage: $0 [Debug|Release]" >&2
    exit 1
fi

DIR="$(dirname "$(realpath "$0")")"
BUILD_DIR=$DIR/build
SPELL_CHECKER=$DIR/spell_checker


rm -rf $BUILD_DIR $SPELL_CHECKER
mkdir $BUILD_DIR

cmake -S "$DIR" -B "$DIR/build" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DBUILD_TESTING=ON -DCMAKE_INSTALL_PREFIX="$DIR"
cmake --build "$DIR/build"
cmake --install "$DIR/build"
