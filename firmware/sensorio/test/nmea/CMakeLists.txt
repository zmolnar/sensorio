set(BINARY nmea-test)
file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)
set(SOURCES ${TEST_SOURCES})
add_executable(${BINARY} ${TEST_SOURCES} ../../src/core/nmea/Processor.cpp
                                         ../../src/core/nmea/Message.cpp
                                         ../../src/core/nmea/Parser.cpp)
add_test(NAME ${BINARY} COMMAND ${BINARY})
target_link_libraries(${BINARY} PUBLIC platform gtest)
