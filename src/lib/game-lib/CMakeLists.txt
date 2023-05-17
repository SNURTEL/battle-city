set(game_lib_dir .)
set(game_lib_sources
        ${game_lib_dir}/Game.cpp
        ${game_lib_dir}/ActiveEventHandler.cpp
        ${game_lib_dir}/PauseEventHandler.cpp
        ${game_lib_dir}/MenuEventHandler.cpp
        ${game_lib_dir}/Menu.cpp
        ${game_lib_dir}/FinishedEventHandler.cpp
        ${game_lib_dir}/GameStatistics.cpp
        ${game_lib_dir}/GameState.cpp
        ${game_lib_dir}/KeyboardController.cpp
        ${game_lib_dir}/GameStatsIO.cpp)

add_library(game-lib ${game_lib_sources})
target_link_libraries(game-lib PRIVATE core-lib board-lib graphic-lib ${SFML_LIBRARIES})

set(game_lib_test_dir ${game_lib_dir}/test)
set(game_lib_test_sources
        ${game_lib_test_dir}/test_game.cpp
        ${game_lib_test_dir}/test_states.cpp
        ${game_lib_test_dir}/test_stats.cpp)

add_executable(test_game_lib ${game_lib_test_sources})
target_link_libraries(test_game_lib PRIVATE game-lib Catch2::Catch2WithMain)

enable_testing()
add_test(GAME_LIB_TEST ./bin/test_game_lib)