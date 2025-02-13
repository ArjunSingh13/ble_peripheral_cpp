# SPDX-License-Identifier: Apache-2.0
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ble_peripheral C CXX)

target_sources(app PRIVATE main.cpp 
    lib/drv_led/drv_led_shell.cpp
    )

set(LED_LIBRARY_NAME drv_led)
set(BLE_PERIPHERAL_LIBRARY_NAME drv_ble_peripheral)

add_subdirectory(external)
add_subdirectory(lib)

# ✅ Ensure ETL finds etl_profile.h
add_definitions(-DETLP_INCLUDE_PROFILE)
include_directories(${CMAKE_SOURCE_DIR}/build/ble_peripheral/_deps/etl-src/include)

target_include_directories(app PRIVATE ${ETL_SOURCE_DIR}/include)
target_link_libraries(app PRIVATE etl::etl)


