cmake_minimum_required(VERSION 3.12)

# stop the compiler tests from running
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

pico_sdk_init()

project(pico_micro_ros_example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

link_directories(libmicroros)
add_executable(pico_micro_ros_example
    pico_micro_ros_example.c
    pico_uart_transport.c
    pico_wifi_transport.c
)
target_link_libraries(pico_micro_ros_example
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_arch
    pico_lwip_core
    microros
)

target_include_directories(pico_micro_ros_example PUBLIC
    libmicroros/include
    ${CMAKE_CURRENT_LIST_DIR}
)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

# Configure Pico
pico_enable_stdio_usb(pico_micro_ros_example 1)
pico_enable_stdio_uart(pico_micro_ros_example 0)
add_compile_definitions(PICO_UART_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_DEFAULT_CRLF=0)

# Generate UF2
pico_add_extra_outputs(pico_micro_ros_example)
