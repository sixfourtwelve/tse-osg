#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

"$ROOT/scripts/build-deps.sh"

cmake \
    -S "$ROOT" \
    -B "$ROOT/build" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_PREFIX_PATH="$ROOT/vendor/install/sdl3;$ROOT/vendor/install/osg"
