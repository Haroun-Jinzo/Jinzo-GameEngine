cmake_minimum_required(VERSION 3.20)
project(Jinzo1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure debug and release builds
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/Libraries/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Graphics
    ${PROJECT_SOURCE_DIR}/src/io
    ${PROJECT_SOURCE_DIR}/src/Physics
    ${PROJECT_SOURCE_DIR}/src/Algorithms
)

# Source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.c
    src/*.hpp
    src/*.h
)

# Add executable
add_executable(Jinzo1.0 ${SOURCES})

# Compiler options
target_compile_options(Jinzo1.0 PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /W3
        /sdl
        /permissive-
    >
)

# Preprocessor definitions
target_compile_definitions(Jinzo1.0 PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    _CONSOLE
)

# Link directories
target_link_directories(Jinzo1.0 PRIVATE
    ${PROJECT_SOURCE_DIR}/Libraries/lib
)

# Dependencies
find_package(OpenGL REQUIRED)

target_link_libraries(Jinzo1.0 PRIVATE
    OpenGL::GL
    glfw3.lib
    assimp-vc142-mtd.lib
    opengl32.lib
)

# Windows subsystem configuration
if(WIN32)
    set_target_properties(Jinzo1.0 PROPERTIES
        WIN32_EXECUTABLE FALSE
        LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE"
    )
endif()

# Copy assimp DLL to build directory (optional)
add_custom_command(TARGET Jinzo1.0 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${PROJECT_SOURCE_DIR}/src/assimp-vc142-mtd.dll
    $<TARGET_FILE_DIR:Jinzo10>
)