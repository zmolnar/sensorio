cmake_minimum_required(VERSION 3.10)

# set the project name
project(UKF)

# add the executable
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(PCSIM)
add_executable(exe ../env/main.cpp ../../sensorio/src/kalmanfilter/matrix.cpp)