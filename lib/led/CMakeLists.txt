set(LIBRARY_SOURCES "led.cpp")

add_library(${LED_LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCES} # only source files here because header files dont create object files so not needed
    ) #add all static files here

message(STATUS "ETL Source Directory: ${ETL_SOURCE_DIR}")
target_include_directories(${LED_LIBRARY_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ) # included header files from this level

target_link_libraries(${LED_LIBRARY_NAME} PUBLIC etl::etl)  