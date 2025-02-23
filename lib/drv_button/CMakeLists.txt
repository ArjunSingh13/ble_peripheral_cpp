set(LIBRARY_SOURCES 
    "drv_button.cpp"
)

add_library(${BUTTON_LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCES} # only source files here because header files dont create object files so not needed
    ) #add all static files here

target_include_directories(${BUTTON_LIBRARY_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ) # included header files from this level

target_include_directories(${BUTTON_LIBRARY_NAME} PUBLIC
    ${ZEPHYR_BASE}/include
    ${ZEPHYR_BASE}/drivers
)


target_link_libraries(${BUTTON_LIBRARY_NAME} PUBLIC 
    zephyr_interface
    etl::etl) 
    
target_link_libraries(app PRIVATE ${BUTTON_LIBRARY_NAME})

