cmake_minimum_required(VERSION 3.10)
project(sensorio-test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_BUILD_TYPE Debug)

add_compile_definitions(PCSIM)

include_directories("../src/")
include_directories("../lib/googletest/googletest/include/")

add_subdirectory(lib/googletest)
add_subdirectory(lib/platform)
add_subdirectory(kalmanfilter)
add_subdirectory(nmea)
