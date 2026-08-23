#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

SDL_SOURCE="$ROOT/vendor/SDL"
SDL_BUILD="$ROOT/vendor/build/sdl3"
SDL_PREFIX="$ROOT/vendor/install/sdl3"
SDL_MARKER="$SDL_PREFIX/.built"

OSG_SOURCE="$ROOT/vendor/OpenSceneGraph"
OSG_BUILD="$ROOT/vendor/build/osg"
OSG_PREFIX="$ROOT/vendor/install/osg"
OSG_MARKER="$OSG_PREFIX/.built"

if [[ ! -f "$SDL_MARKER" ]]; then
    echo "Configuring SDL3..."

    mkdir -p "$SDL_BUILD" "$SDL_PREFIX"

    cmake \
        -S "$SDL_SOURCE" \
        -B "$SDL_BUILD" \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="$SDL_PREFIX" \
        -DSDL_SHARED=ON \
        -DSDL_STATIC=OFF \
        -DSDL_TESTS=OFF \
        -DSDL_EXAMPLES=OFF

    echo "Building SDL3..."
    cmake --build "$SDL_BUILD"

    echo "Installing SDL3..."
    cmake --install "$SDL_BUILD"

    touch "$SDL_MARKER"
else
    echo "SDL3 already built."
fi

if [[ ! -f "$OSG_MARKER" ]]; then
    echo "Configuring OpenMW OpenSceneGraph fork..."

    mkdir -p "$OSG_BUILD" "$OSG_PREFIX"

    cmake \
      -S "$OSG_SOURCE" \
      -B "$OSG_BUILD" \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="$OSG_PREFIX" \
      -DOPENGL_PROFILE=GL3 \
      -DBUILD_OSG_APPLICATIONS=OFF \
      -DBUILD_OSG_PLUGINS_BY_DEFAULT=OFF \
      -DBUILD_OSG_PLUGIN_OSG=ON \
      -DBUILD_OSG_PLUGIN_IMAGEIO=ON \
      -DBUILD_OSG_PLUGIN_OBJ=ON \
      -DBUILD_OSG_PLUGIN_PNG=ON \
      -DBUILD_OSG_PLUGIN_JPEG=ON \
      -DBUILD_OSG_PLUGIN_DDS=ON \
      -DBUILD_OSG_PLUGIN_TGA=ON \
      -DBUILD_OSG_EXAMPLES=OFF \
      -DBUILD_DOCUMENTATION=OFF \
      -DBUILD_OSG_PLUGINS_BY_DEFAULT=OFF \
      -DBUILD_OSG_PLUGIN_FFMPEG=OFF \
      -DBUILD_OSG_DEPRECATED_SERIALIZERS=OFF    
    
    echo "Building OpenSceneGraph..."
    cmake --build "$OSG_BUILD"

    echo "Installing OpenSceneGraph..."
    cmake --install "$OSG_BUILD"

    touch "$OSG_MARKER"
else
    echo "OpenSceneGraph already built."
fi

echo
echo "Dependencies are ready."
echo "Normal application builds will not rebuild them."
