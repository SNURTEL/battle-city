
set(board_lib_dir .)
set(board_lib_sources
        ${board_lib_dir}/Grid.cpp
        ${board_lib_dir}/TileManager.cpp
        ${board_lib_dir}/Board.cpp
        ${board_lib_dir}/GridBuilder.cpp
        ${board_lib_dir}/Eagle.cpp)

add_library(board-lib ${board_lib_sources})
target_link_libraries(board-lib PRIVATE tank-lib game-lib)

set(board_lib_test_dir ${board_lib_dir}/test)
set(board_lib_test_sources
        ${board_lib_test_dir}/test_grid.cpp
        ${board_lib_test_dir}/test_board.cpp)

add_executable(test_board_lib ${board_lib_test_sources})
target_link_libraries(test_board_lib PRIVATE board-lib Catch2::Catch2WithMain)
add_test(NAME test_board_lib COMMAND test_board_lib)

enable_testing()
add_test(BOARD_LIB_TEST ./bin/test_board_lib)