target_sources(app PRIVATE "app.cpp")

target_include_directories(app PRIVATE ${ETL_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})