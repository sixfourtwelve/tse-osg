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

set(IMGUI_SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui")

add_library(imgui STATIC
    "${IMGUI_SOURCE_DIR}/imgui.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_demo.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_draw.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_tables.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_widgets.cpp"
    "${IMGUI_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
    "${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)
add_library(imgui::imgui ALIAS imgui)

target_include_directories(imgui
    SYSTEM PUBLIC
        "${IMGUI_SOURCE_DIR}"
        "${IMGUI_SOURCE_DIR}/backends"
)

target_link_libraries(imgui
    PUBLIC
        SDL3::SDL3
        OpenGL::GL
)


file(GLOB_RECURSE COMPONENT_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/components/*.cpp"
)

file(GLOB_RECURSE COMPONENT_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/components/*.hpp"
)

if(COMPONENT_SOURCES)
    add_library(tse_components STATIC
        ${COMPONENT_SOURCES}
        ${COMPONENT_HEADERS}
    )
    target_include_directories(tse_components
        PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}"
    )
else()
    add_library(tse_components INTERFACE)
    target_sources(tse_components INTERFACE ${COMPONENT_HEADERS})
    target_include_directories(tse_components
        INTERFACE
            "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

target_link_libraries(tse_components
    PUBLIC
        SDL3::SDL3
        ${OPENSCENEGRAPH_LIBRARIES}
)

file(GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/apps/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/apps/*.hpp"
)

add_executable(tse ${SOURCE_FILES})

if(APPLE)
  target_compile_options(tse PRIVATE
    "$<$<COMPILE_LANG_AND_ID:CXX,AppleClang>:-Wno-#warnings>"
  )

  add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()

target_include_directories(tse
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/apps/tse"
)

target_include_directories(tse
    SYSTEM PRIVATE
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

target_link_libraries(tse
    PRIVATE
        imgui::imgui
        tse_components
        ${OPENSCENEGRAPH_LIBRARIES}
)

if(APPLE)
    set_target_properties(tse PROPERTIES
        BUILD_RPATH
            "${CMAKE_SOURCE_DIR}/vendor/install/sdl3/lib;${CMAKE_SOURCE_DIR}/vendor/install/osg/lib"
    )
endif()

add_custom_target(tse_assets ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf
            "${CMAKE_CURRENT_BINARY_DIR}/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "${CMAKE_CURRENT_BINARY_DIR}/assets"
    COMMENT "Copying application assets"
)
add_dependencies(tse tse_assets)
