cmake_minimum_required(VERSION 3.24)

project(tse LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


list(PREPEND CMAKE_PREFIX_PATH
    "${CMAKE_SOURCE_DIR}/vendor/install/sdl3"
    "${CMAKE_SOURCE_DIR}/vendor/install/osg"
)

find_package(OpenGL REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

find_package(OpenSceneGraph REQUIRED
    COMPONENTS
        osg
        osgDB
        osgGA
        osgUtil
        osgViewer
)

add_executable(tse
    src/main.cpp
)

if(APPLE)
  target_compile_options(tse PRIVATE
    "$<$<COMPILE_LANG_AND_ID:CXX,AppleClang>:-Wno-#warnings>"
  )

  add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()

target_include_directories(tse
  SYSTEM PRIVATE
  ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

target_link_libraries(tse
    PRIVATE
        SDL3::SDL3
        ${OPENSCENEGRAPH_LIBRARIES}
        OpenGL::GL
)

if(APPLE)
    set_target_properties(tse PROPERTIES
        BUILD_RPATH
            "${CMAKE_SOURCE_DIR}/vendor/install/sdl3/lib;${CMAKE_SOURCE_DIR}/vendor/install/osg/lib"
    )
endif()

add_custom_command(
    TARGET tse POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/shaders"
            "$<TARGET_FILE_DIR:tse>/shaders"
)
