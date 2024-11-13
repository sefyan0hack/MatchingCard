cmake_minimum_required(VERSION 3.25)

project(CardMatching LANGUAGES CXX)

# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define warning flags
set(WAR_PANIC "-Wall -Wextra -pedantic")

# Compiler-specific flags
set(CMAKE_CXX_FLAGS_RELEASE "${WAR_PANIC} -O3 -march=native -mwindows")  # Optimize for GCC/Clang
set(CMAKE_CXX_FLAGS_DEBUG "${WAR_PANIC} -O0 -g3 -ggdb ")  # Debugging for GCC/Clang

# Add the -static flag for complete static linking
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

set(BUILD_SHARED_LIBS OFF)  # Turn off shared library builds
add_subdirectory(${CMAKE_SOURCE_DIR}/raylib-master)

add_executable(CardMatching src/main.cpp)
target_link_libraries(CardMatching raylib)

include_directories("${CMAKE_SOURCE_DIR}/res")
