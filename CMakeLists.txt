cmake_minimum_required(VERSION 3.15.1)
project(PROI_PROJEKT)
set(CMAKE_CXX_STANDARD 20)

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.0.1)
FetchContent_GetProperties(Catch2)
FetchContent_MakeAvailable(Catch2)

set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(SFML)

include_directories(${SFML_SOURCE_DIR}/include)


set(CMAKE_BINARY_DIR bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SFML_LIBRARIES sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio)

add_subdirectory(src/lib/core-lib)
add_subdirectory(src/lib/game-lib)
add_subdirectory(src/lib/tank-lib)
add_subdirectory(src/lib/board-lib)
add_subdirectory(src/lib/graphic-lib)
add_subdirectory(src/lib/bot-lib)

enable_testing()

add_executable(tanks src/tanks.cpp)
include_directories(src/lib/game-lib/include)
target_link_libraries(tanks ${SFML_LIBRARIES} core-lib game-lib tank-lib board-lib bot-lib)

file(COPY res DESTINATION "${CMAKE_BINARY_DIR}")