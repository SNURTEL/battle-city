cmake_minimum_required(VERSION 3.16.3)
project(PROI_PROJEKT)
set(CMAKE_CXX_STANDARD 20)
find_package(Catch2 REQUIRED)


set(CMAKE_BINARY_DIR ../../bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

############################ CORE-LIB ################################

set(core_lib_dir ../src/core-lib)
set(core_lib_sources
        ${core_lib_dir}/Clock.cpp
        ${core_lib_dir}/Event.cpp
        ${core_lib_dir}/EventQueue.cpp
        ${core_lib_dir}/SimplePublisher.cpp
        ${core_lib_dir}/SimpleSubscriber.cpp
        ${core_lib_dir}/ObserverExceptions.cpp
        ${core_lib_dir}/EventPublisher.cpp
        ${core_lib_dir}/EventSubscriber.cpp
        ${core_lib_dir}/EventHandler.cpp
        ${core_lib_dir}/SingletonExceptions.cpp
        )

add_library(core-lib ${core_lib_sources})

set(core_lib_test_dir ../src/core-lib/test)
set(core_lib_test_sources
        ${core_lib_test_dir}/test_simpleObserver.cpp
        ${core_lib_test_dir}/test_eventObserver.cpp
        ${core_lib_test_dir}/test_eventQueue.cpp ..
        ${core_lib_test_dir}/test_event.cpp)

add_executable(test_core_lib ${core_lib_test_sources})
target_link_libraries(test_core_lib PRIVATE core-lib Catch2::Catch2WithMain)

############################ GAME-LIB ################################

set(game_lib_dir ../src/game-lib)
set(game_lib_sources
        ${game_lib_dir}/Game.cpp
        ../src/game-lib/Game.cpp ../src/game-lib/include/Game.h)

add_library(game-lib ${game_lib_sources})

set(game_lib_test_dir ../src/game-lib/test)
set(game_lib_test_sources
        ../src/game-lib/test/test_sanity.cpp)

add_executable(test_game_lib ${game_lib_test_sources})
target_link_libraries(test_game_lib PRIVATE game-lib Catch2::Catch2WithMain)

############################ MAIN ################################


add_executable(tanks ../src/main/tanks.cpp)
target_link_libraries(tanks PRIVATE core-lib)

