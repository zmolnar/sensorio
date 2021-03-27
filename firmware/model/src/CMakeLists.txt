set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})
list(APPEND SOURCES "../../sensorio/src/kalmanfilter/Matrix.cpp")
list(APPEND SOURCES "../../sensorio/src/kalmanfilter/Ukf.cpp")
list(APPEND SOURCES "../../sensorio/src/kalmanfilter/MerweScaledSigmaPoints.cpp")

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})