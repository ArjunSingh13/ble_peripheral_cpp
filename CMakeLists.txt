# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ble_peripheral)

#set(EXECUTABLE_NAME Executable)
set(LED_LIBRARY_NAME "LedLibrary")

add_subdirectory(external)
add_subdirectory(lib)
add_subdirectory(app)

