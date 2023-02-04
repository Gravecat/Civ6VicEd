# Simple CMake build file for Civ6 Victory Editor
# For MinGW/MSYS builds, be sure to specify -G"MinGW Makefiles" when setting up the build folder.
# Copyright © 2023 Raine "Gravecat" Simmons. Licensed under the MIT License.

cmake_minimum_required(VERSION 3.13)
project(civ6viced)

# Automatically default to release builds if a type is not specified.
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Build type was not specified, switching to 'Release'.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake.
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
set(CMAKE_CXX_FLAGS_RELEASE  "-Os")

add_executable(civ6viced civ6viced.cpp)

if(WIN32)
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s -static")
  target_link_libraries(civ6viced mingw32)
else()
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s -static -Wl,-rpath='${ORIGIN}'")
endif(WIN32)
