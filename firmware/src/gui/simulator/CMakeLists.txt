cmake_minimum_required(VERSION 3.16)

project(sensorio_gui_simulator LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(FIRMWARE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../..")
set(SRC_DIR "${FIRMWARE_DIR}/src")
set(LVGL_DIR "${FIRMWARE_DIR}/lib/lvgl")
set(ETL_DIR "${FIRMWARE_DIR}/lib/etl")
set(SIMULATOR_DIR "${CMAKE_CURRENT_LIST_DIR}")

find_program(SDL2_CONFIG sdl2-config)
if(NOT SDL2_CONFIG)
    message(FATAL_ERROR "sdl2-config not found. Install SDL2 before building the simulator.")
endif()
execute_process(
    COMMAND "${SDL2_CONFIG}" --cflags
    OUTPUT_VARIABLE SDL2_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND "${SDL2_CONFIG}" --libs
    OUTPUT_VARIABLE SDL2_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND "${SDL2_CONFIG}" --prefix
    OUTPUT_VARIABLE SDL2_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
separate_arguments(SDL2_CFLAGS_LIST UNIX_COMMAND "${SDL2_CFLAGS}")
separate_arguments(SDL2_LIBS_LIST UNIX_COMMAND "${SDL2_LIBS}")

file(GLOB_RECURSE LVGL_SOURCES CONFIGURE_DEPENDS "${LVGL_DIR}/src/*.c")

add_executable(sensorio-gui-simulator
    "${SIMULATOR_DIR}/src/main.cpp"
    "${SIMULATOR_DIR}/hal/app_hal.c"
    "${SIMULATOR_DIR}/drivers/monitor.c"
    "${SIMULATOR_DIR}/drivers/keyboard.c"
    "${SIMULATOR_DIR}/stubs/Power.cpp"
    "${SIMULATOR_DIR}/stubs/DataLoggerThread.cpp"
    "${SRC_DIR}/dashboard/Config.cpp"
    "${SRC_DIR}/dashboard/Gps.cpp"
    "${SRC_DIR}/dashboard/RawSerializer.cpp"
    "${SRC_DIR}/gui/Sensorio.cpp"
    "${SRC_DIR}/gui/images/paraglider.c"
    "${SRC_DIR}/gui/screens/BpsData.cpp"
    "${SRC_DIR}/gui/screens/GpsData.cpp"
    "${SRC_DIR}/gui/screens/ImuData.cpp"
    "${SRC_DIR}/gui/screens/Startup.c"
    "${SRC_DIR}/gui/screens/SystemStatus.cpp"
    "${SRC_DIR}/gui/screens/Variometer.cpp"
    "${SRC_DIR}/gui/screens/VariometerSettings.cpp"
    "${SRC_DIR}/platform/native/Assert.cpp"
    "${SRC_DIR}/platform/native/Log.cpp"
    ${LVGL_SOURCES}
)

target_include_directories(sensorio-gui-simulator PRIVATE
    "${SRC_DIR}"
    "${SIMULATOR_DIR}/drivers"
    "${SIMULATOR_DIR}/hal"
    "${SIMULATOR_DIR}/src"
    "${LVGL_DIR}"
    "${LVGL_DIR}/src"
    "${ETL_DIR}/include"
    "${SDL2_PREFIX}/include"
)

target_compile_definitions(sensorio-gui-simulator PRIVATE
    SIMULATOR
    LV_CONF_INCLUDE_SIMPLE
    LV_LVGL_H_INCLUDE_SIMPLE
)

target_compile_options(sensorio-gui-simulator PRIVATE
    -Wall
    ${SDL2_CFLAGS_LIST}
)
target_link_libraries(sensorio-gui-simulator PRIVATE ${SDL2_LIBS_LIST})
