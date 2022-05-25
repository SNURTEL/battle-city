cmake_minimum_required(VERSION 3.16.3)
project(PROI_PROJEKT)
set(CMAKE_CXX_STANDARD 20)
find_package(Catch2 REQUIRED)

set(CMAKE_BINARY_DIR ../../bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# SFML stuff
find_package(SFML REQUIRED system window graphics network audio)

set(SFML_INCLUDE_DIR ./SFML-2.5.1/include/SFML/System.hpp)
set(SFML_LIBRARIES sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio)

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
        ${game_lib_dir}/ActiveEventHandler.cpp
        ${game_lib_dir}/GameState.cpp
        ${game_lib_dir}/KeyboardController.cpp)

add_library(game-lib ${game_lib_sources})
target_link_libraries(game-lib PRIVATE core-lib ${SFML_LIBRARIES})

set(game_lib_test_dir ../src/game-lib/test)
set(game_lib_test_sources
        ${game_lib_test_dir}/test_sanity.cpp)

add_executable(test_game_lib ${game_lib_test_sources})
target_link_libraries(test_game_lib PRIVATE game-lib Catch2::Catch2WithMain)

############################ TANK-LIB ################################

set(tank_lib_dir ../src/tank-lib)
set(tank_lib_sources
        ${tank_lib_dir}/Tank.cpp
        ${tank_lib_dir}/EntityController.cpp
        )

add_library(tank-lib ${tank_lib_sources} ../src/tank-lib/Bullet.cpp ../src/tank-lib/include/Bullet.h ../src/tank-lib/Entity.cpp ../src/tank-lib/include/Entity.h)
target_link_libraries(tank-lib PRIVATE game-lib)

set(tank_lib_test_dir ../src/tank-lib/test)
set(tank_lib_test_sources
        ${tank_lib_test_dir}/test_tank.cpp
        ${tank_lib_test_dir}/test_entityController.cpp)

add_executable(test_tank_lib ${tank_lib_test_sources})
target_link_libraries(test_tank_lib PRIVATE tank-lib Catch2::Catch2WithMain)

############################ BOARD-LIB ################################

set(board_lib_dir ../src/board-lib)
set(board_lib_sources
        ${board_lib_dir}/Grid.cpp
        ${board_lib_dir}/TileManager.cpp
        ${board_lib_dir}/Board.cpp .
        ${board_lib_dir}/GridBuilder.cpp)

add_library(board-lib ${board_lib_sources})
target_link_libraries(board-lib PRIVATE tank-lib game-lib)

set(board_lib_test_dir ../src/board-lib/test)
set(board_lib_test_sources
        ${board_lib_test_dir}/test_grid.cpp
        ${board_lib_test_dir}/test_board.cpp)

add_executable(test_board_lib ${board_lib_test_sources})
target_link_libraries(test_board_lib PRIVATE board-lib Catch2::Catch2WithMain)

######################### GRAPHIC-LIB ####################################
set(graphic_lib_dir ../src/graphic-lib)
# set(graphic_lib_sources
#         ${graphic_lib_dir}/Tank.cpp
#         ${graphic_lib_dir}/TankController.cpp
#         )

# add_library(tank-lib ${tank_lib_sources})
# target_link_libraries(graphic-lib PRIVATE game-lib tank-lib)

# set(grap_lib_test_dir ../src/graphic-lib/test)
# set(graphic_lib_test_source
#         ${graphic_lib_test_dir}/test_tank.cpp
#         ${graphic_lib_test_dir}/test_tankController.cpp)

# add_executable(test_graphic_lib ${graphic_lib_test_sources})
# target_link_libraries(test_graphic_lib PRIVATE graphic-lib Catch2::Catch2WithMain)
############################ MAIN ################################


add_executable(tanks ../src/main/tanks.cpp ${core_lib_sources} ${game_lib_sources} ${tank_lib_sources})
target_link_libraries(tanks ${SFML_LIBRARIES} core-lib game-lib tank-lib)

