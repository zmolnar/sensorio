cmake_minimum_required(VERSION 3.10)
project(sensorio-cppmodel)

set(CMAKE_CXX_STANDARD 11)

add_compile_definitions(PCSIM)

include_directories("../sensorio/src/kalmanfilter")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/googletest)
