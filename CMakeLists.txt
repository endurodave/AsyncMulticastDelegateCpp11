# Example CMake command line to create project build files:
#
# *** Windows ***
# cmake -G "Visual Studio 17 2022" -A Win32 -B Build -S .
# cmake -G "Visual Studio 17 2022" -A x64 -B Build -S .
# cmake -G "Visual Studio 17 2022" -A x64 -B Build -S . -DENABLE_UNIT_TESTS=ON
#
# *** Linux ***
# cmake -G "Unix Makefiles" -B Build -S .
# cmake -G "Unix Makefiles" -B Build -S . -DENABLE_UNIT_TESTS=ON

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language (C or C++)
project(Delegate VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect all .cpp and *.h source files in the current directory
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.h")

# Add subdirectories to include path
include_directories( 
    ${CMAKE_SOURCE_DIR}/Delegate
    ${CMAKE_SOURCE_DIR}/Examples
    ${CMAKE_SOURCE_DIR}/Port
)

# Add an executable target
add_executable(DelegateApp ${SOURCES})

# Define the DELEGATE_UNIT_TEST macro for the DelegateApp target
if (ENABLE_UNIT_TESTS)
    add_compile_definitions(DELEGATE_UNIT_TESTS)
endif()

# Add subdirectories to build
add_subdirectory(Delegate)
add_subdirectory(Examples)
add_subdirectory(Port)

target_link_libraries(DelegateApp PRIVATE 
    DelegateLib
    ExamplesLib
    PortLib
)

