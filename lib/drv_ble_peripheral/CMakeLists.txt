set(LIBRARY_SOURCES 
    "drv_ble_advertise.cpp"
    "drv_ble_shell.cpp"
)

add_library(${BLE_PERIPHERAL_LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCES} # only source files here because header files dont create object files so not needed
    ) #add all static files here

target_include_directories(${BLE_PERIPHERAL_LIBRARY_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ) # included header files from this level

target_include_directories(${BLE_PERIPHERAL_LIBRARY_NAME} PUBLIC
    ${ZEPHYR_BASE}/include
    ${ZEPHYR_BASE}/drivers
)


target_link_libraries(${BLE_PERIPHERAL_LIBRARY_NAME} PUBLIC 
    zephyr_interface
    etl::etl) 
    
target_link_libraries(app PRIVATE ${BLE_PERIPHERAL_LIBRARY_NAME})

