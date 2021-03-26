cmake_minimum_required(VERSION 3.10)

# set the project name
project(UKF)

# add the executable

set(SOURCEFILES "../src/main.cpp")
list(APPEND SOURCEFILES "../../sensorio/src/kalmanfilter/matrix.cpp")
list(APPEND SOURCEFILES "../../sensorio/src/kalmanfilter/ukf.cpp")
list(APPEND SOURCEFILES "../../sensorio/src/kalmanfilter/MerweScaledSigmaPoints.cpp")

set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(PCSIM)

add_executable(exe  ${SOURCEFILES})