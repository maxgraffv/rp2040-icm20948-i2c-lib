cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "/opt/pico-sdk")

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(ICM20948 C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_C_COMPILER arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
#set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

pico_sdk_init()

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(icm20948_i2c 
    src/icm20948_i2c.c
)


target_link_libraries(icm20948_i2c pico_stdlib hardware_i2c)

add_executable(main example/main.c)

target_link_libraries(main icm20948_i2c)

pico_add_extra_outputs(main)
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
