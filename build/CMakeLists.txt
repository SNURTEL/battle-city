cmake_minimum_required(VERSION 3.16.3)
project(PROI_PROJEKT)
set(CMAKE_CXX_STANDARD 20)
find_package(Catch2 REQUIRED)


set(CMAKE_BINARY_DIR ../../bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(core_lib_dir ../src/core-lib)
set(core_lib_sources ${core_lib_dir}/Clock.cpp
        ${core_lib_dir}/Event.cpp
        ${core_lib_dir}/EventQueue.cpp
        ${core_lib_dir}/Publisher.cpp
        ${core_lib_dir}/Subscriber.cpp)

add_library(core-lib ${core_lib_sources})

add_executable(tanks ../src/main/tanks.cpp)
target_link_libraries(tanks PRIVATE core-lib)