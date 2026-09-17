cmake_minimum_required(VERSION 3.20)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed. Please run 'cmake -B build' and 'cmake --build build'.")
endif()

project(unNamedEngine VERSION 0.01 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 REQUIRED CONFIG)

# --- Sources ---
set(SOURCES
    src/main.cpp
    # Core Engine
    #src/engine/core/Engine.cpp
    #src/engine/core/LogSystem.cpp
    #src/engine/core/CommandBus.cpp
    src/engine/core/WindowSystem.cpp
    src/engine/memory/Arena.cpp
    #src/engine/scene/SceneManager.cpp
    #src/engine/platform/InputSystem.cpp
    #src/engine/platform/EnginePlatform.cpp
    #src/engine/platform/TimeSystem.cpp
    #src/engine/ecs/EntityManager.cpp
    #src/engine/ecs/EntityAPI.cpp
    #src/engine/ecs/EntitySystem.cpp
    #src/engine/systems/audio/AudioSystem.cpp
    #src/engine/systems/audio/AudioAPI.cpp
    #src/engine/systems/animation/AnimationSystem.cpp
    #src/engine/systems/animation/AnimationAPI.cpp
    #src/engine/systems/camera/CameraAPI.cpp
    #src/engine/systems/physics/PhysicsAPI.cpp

    # Game Test
    #src/game/game.cpp
    #src/game/game_scenes.cpp
    #src/game/controlSystem.cpp
    #src/game/game_prototypes.cpp
    #src/game/menu.cpp
    #src/game/game_over.cpp
)

# --- Executable ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- Link Time Optimization (LTO) ---
# This allows inlining functions across translation units
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  include(CheckIPOSupported)
  check_ipo_supported(RESULT result OUTPUT output)

  if(result)
    message(STATUS "IPO/LTO is supported: Enabling")
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(WARNING "IPO/LTO is not supported: ${output}")
  endif()
endif()

# --- EXTERNAL LIBRARIES (Vendored) ---
add_library(miniaudio_lib STATIC src/external/miniaudio/miniaudio_impl.cpp)
add_library(fmt_lib STATIC src/external/fmt/src/format.cc)

target_link_libraries(miniaudio_lib PRIVATE m pthread dl SDL3::SDL3)

# --- PROJECT DIRECTORIES & LINKING ---
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src) )
target_include_directories(fmt_lib PUBLIC src/external/fmt/include)
target_link_libraries(${PROJECT_NAME}  miniaudio_lib fmt_lib SDL3::SDL3)

# --- Debug and Release modes ---
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Build Type is Debug: Enabling Strict C++20 Warnings & Sanitizers")

  if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX:vla /permissive-)
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic -Wshadow
        -Werror=vla -Wdouble-promotion -Wnull-dereference
        -Wimplicit-fallthrough -Wundef -Wformat=2
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -Wshadow -Wswitch-enum
        -Wundef -Wformat=2
        -Wconversion -Wsign-conversion
        -Wold-style-cast -Wcast-qual
        -Werror
    )

    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
  endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "Build Type is Release: Enabling Aggressive Optimizations")

  if(MSVC)
    # Visual Studio Flags
    # /Ox = Full Optimization
    # /arch:AVX2 = Enable SIMD vectorization (Crucial for SoA)
    # /fp:fast = Faster floating point math (less precise, usually fine for games)
    target_compile_options(${PROJECT_NAME} PRIVATE /Ox /arch:AVX2 /fp:fast)
  else()
    # GCC / Clang Flags
    # -O3 = Maximum Optimization
    # -march=native = Use your specific CPU's instruction set (AVX/AVX2)
    # -ffast-math = trade IEEE precision for speed
    # -funroll-loops = Helps with the physics "for" loops
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -march=native -ffast-math -funroll-loops)
  endif()
endif()
