cmake_minimum_required(VERSION 3.25)

project(ResMan LANGUAGES CXX)

# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define warning flags
set(WAR_PANIC "-Wall -Wextra -pedantic")

# Compiler-specific flags
set(CMAKE_CXX_FLAGS_RELEASE "${WAR_PANIC} -O2 -s -flto -fdata-sections -ffunction-sections -Wl,--gc-sections -mwindows")  # Optimize for GCC/Clang
set(CMAKE_CXX_FLAGS_DEBUG "${WAR_PANIC} -O0 -g3 -ggdb ")  # Debugging for GCC/Clang


add_executable(ResMan main.cpp)

# set_target_properties( ResMan
#     PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
# )