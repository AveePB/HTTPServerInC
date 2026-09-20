cmake_minimum_required(VERSION 3.10.0)
project(HTTPServer VERSION 0.1.0 LANGUAGES C)

# Create server library 
file(GLOB SOURCES "src/*.c")
add_library(ServerLib ${SOURCES})
target_include_directories(ServerLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link project
add_executable(HTTPServer src/main.c)
target_link_libraries(HTTPServer PRIVATE ServerLib)
