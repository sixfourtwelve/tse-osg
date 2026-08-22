#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if [[ ! -f "$ROOT/build/build.ninja" ]]; then
    "$ROOT/scripts/configure.sh"
fi

cmake --build "$ROOT/build" -j16
