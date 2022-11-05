cmake_minimum_required(VERSION 3.10)

project(main)

# for filesystem functionality from C++17
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

if (WIN32)
    set(WHERE-IS-SDL "c:/programs/sdl/lib/x64")
    set(WHERE-IS-SDL-HEADERS "c:/programs/sdl/include")
    set(WHERE-IS-GLAD "c:/programs/glad/src/glad.c")
    set(WHERE-IS-GLAD-HEADERS "c:/programs/glad/include")
endif()

if (UNIX)
    set(WHERE-IS-GLAD "${CMAKE_SOURCE_DIR}/../my_dependencies/glad/src/glad.c")
    set(WHERE-IS-GLAD-HEADERS "${CMAKE_SOURCE_DIR}/../my_dependencies/glad/include")

    if (APPLE)
        set(WHERE-IS-SDL "${CMAKE_SOURCE_DIR}/../my_dependencies")
        set(WHERE-IS-SDL-HEADERS "${CMAKE_SOURCE_DIR}/../my_dependencies/SDL2.framework/Versions/Current/Headers")
    endif()

    if(NOT APPLE)
        find_package(SDL2 REQUIRED)
        include_directories(${SDL2_INCLUDE_DIRS})
    endif()
endif()

message(${WHERE-IS-SDL})
message(${WHERE-IS-SDL-HEADERS})
message(${WHERE-IS-GLAD})
message(${WHERE-IS-GLAD-HEADERS})

if (WIN32 OR APPLE)
    find_library(SDL SDL2 PATHS ${WHERE-IS-SDL})
    if (WIN32)
        find_library(SDLmain SDL2main PATHS ${WHERE-IS-SDL})
    endif()

    include_directories(
        "${WHERE-IS-SDL-HEADERS}"
    )
endif()

add_library("glad" ${WHERE-IS-GLAD})
include_directories(
        "${WHERE-IS-GLAD-HEADERS}"
    )

message(${SDL})

# your sources
set(sources
    src/main.cpp
    src/Renderer.cpp
    src/VertexBuffer.cpp
    src/IndexBuffer.cpp
    src/VertexArray.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/tests/TestClearColour.cpp
    src/tests/Test.cpp
    src/tests/TestTexture2D.cpp
    ../my_dependencies/imgui-docking/imconfig.h
    ../my_dependencies/imgui-docking/imgui.cpp
    ../my_dependencies/imgui-docking/imgui.h
    ../my_dependencies/imgui-docking/imgui_demo.cpp
    ../my_dependencies/imgui-docking/imgui_draw.cpp
    ../my_dependencies/imgui-docking/imgui_internal.h
    ../my_dependencies/imgui-docking/imgui_widgets.cpp
    ../my_dependencies/imgui-docking/imstb_rectpack.h
    ../my_dependencies/imgui-docking/imstb_textedit.h
    ../my_dependencies/imgui-docking/imstb_truetype.h
    ../my_dependencies/imgui-docking/backends/imgui_impl_opengl3.cpp
    ../my_dependencies/imgui-docking/backends/imgui_impl_opengl3.h
    ../my_dependencies/imgui-docking/backends/imgui_impl_sdl.cpp
    ../my_dependencies/imgui-docking/backends/imgui_impl_sdl.h
)

add_library(imgui
    # Main Imgui files
    ../my_dependencies/imgui-docking/imgui.cpp ../my_dependencies/imgui-docking/imgui_draw.cpp
    ../my_dependencies/imgui-docking/imgui_tables.cpp ../my_dependencies/imgui-docking/imgui_widgets.cpp
    # SDL2+OpenGL-specific files
    ../my_dependencies/imgui-docking/backends/imgui_impl_sdl.cpp
    ../my_dependencies/imgui-docking/backends/imgui_impl_opengl3.cpp)

# Make SDL2 available to Imgui
target_include_directories(imgui PUBLIC ../my_dependencies/SDL2.framework/Versions/A/Headers)
# imgui/backends/ contains the SDL implementation
target_include_directories(imgui PUBLIC ../my_dependencies/imgui-docking/ ../my_dependencies/imgui-docking/backends/)

if (WIN32)
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${sources})
    target_link_libraries(${CMAKE_PROJECT_NAME}
        "glad"
        ${SDL}
        ${SDLmain}
        ${OPENGL_gl_LIBRARY}
        )
else()
    add_executable(${CMAKE_PROJECT_NAME} ${sources})
    target_link_libraries(${CMAKE_PROJECT_NAME}
            "glad"
            ${OPENGL_gl_LIBRARY}
            imgui
            )
    if (APPLE)
        target_link_libraries(${CMAKE_PROJECT_NAME}
            ${SDL}
            )
    endif()
    if (UNIX AND NOT APPLE)
        target_link_libraries(${CMAKE_PROJECT_NAME}
            ${SDL2_LIBRARIES}
            ${CMAKE_DL_LIBS}
            )
    endif()
endif()

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "include" "vendor/stb_image" "vendor/glm" "include/tests")