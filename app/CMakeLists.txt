
target_sources(app PRIVATE "main.cpp")

target_include_directories(app PRIVATE ${ETL_SOURCE_DIR}/include)
target_link_libraries(app PRIVATE ${LED_LIBRARY_NAME} etl::etl)