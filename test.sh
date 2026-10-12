#!/usr/bin/bash
set -e

DIR="$(dirname "$(realpath "$0")")"

ctest --test-dir "$DIR/build" --output-on-failure