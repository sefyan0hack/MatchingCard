cmake_minimum_required(VERSION 3.25)

project(CardMatching LANGUAGES CXX)

# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add CPM as pack man
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.3/CPM.cmake
    ${CMAKE_SOURCE_DIR}/cmake/CPM.cmake
    EXPECTED_HASH SHA256=2e89a807e906eab270c68c531129dc6b7f989c79a38a4cca7dde0f3ec93d9c68
)
endif()

# Include CPM
if(NOT CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cache/CPM" CACHE STRING "CPM source cache")
endif()

include(cmake/CPM.cmake)

CPMAddPackage("gh:raysan5/raylib#5.5")

# Define warning flags
set(WAR_PANIC "-Wall -Wextra -pedantic")

# Compiler-specific flags
set(CMAKE_CXX_FLAGS_RELEASE "${WAR_PANIC} -O2 -s -flto -fdata-sections -ffunction-sections -Wl,--gc-sections -mwindows")  # Optimize for GCC/Clang
set(CMAKE_CXX_FLAGS_DEBUG "${WAR_PANIC} -O0 -g3 -ggdb ")  # Debugging for GCC/Clang

if(WIN32)
    # Add the -static flag for complete static linking
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()
set(RES_DIR ${CMAKE_SOURCE_DIR}/res)
set(RES_BIN ${CMAKE_BINARY_DIR}/res)

add_subdirectory(${RES_DIR})

# set(BUILD_SHARED_LIBS OFF)  # Turn off shared library builds
# add_subdirectory(${CMAKE_SOURCE_DIR}/raylib-master)

add_executable(CardMatching src/main.cpp)

target_link_libraries(CardMatching raylib)

target_include_directories(CardMatching PUBLIC "${CMAKE_SOURCE_DIR}/res")

add_custom_target(PrepareResDir
    COMMAND ${RES_BIN}/ResMan .
    WORKING_DIRECTORY ${RES_DIR}
    COMMENT "Preparing resources in the RES_DIR"
)

add_dependencies(CardMatching PrepareResDir)
add_dependencies(CardMatching ResMan)