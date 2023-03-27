cmake_minimum_required(VERSION 3.23)
project(practice01)

set(CMAKE_CXX_STANDARD 20)

# Copy "database.txt" to the build directory
configure_file(database.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(practice01 main01.cpp)
