cmake_minimum_required(VERSION 3.16.3)
project(PROI_PROJEKT)
set(CMAKE_CXX_STANDARD 20)
find_package(Catch2 REQUIRED)


set(CMAKE_BINARY_DIR ../../bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_executable(tanks ../src/main/tanks.cpp)

