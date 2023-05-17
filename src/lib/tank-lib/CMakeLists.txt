set(tank_lib_dir .)
set(tank_lib_sources
        ${tank_lib_dir}/Tank.cpp
        ${tank_lib_dir}/EntityController.cpp
        ${tank_lib_dir}/Bullet.cpp
        ${tank_lib_dir}/Entity.cpp
        )

add_library(tank-lib ${tank_lib_sources})
target_link_libraries(tank-lib PRIVATE game-lib bot-lib)

set(tank_lib_test_dir ${tank_lib_dir}/test)
set(tank_lib_test_sources
        ${tank_lib_test_dir}/test_tank.cpp
        ${tank_lib_test_dir}/test_entityController.cpp)

add_executable(test_tank_lib ${tank_lib_test_sources})
target_link_libraries(test_tank_lib PRIVATE tank-lib Catch2::Catch2WithMain)

enable_testing()
add_test(TANK_LIB_TEST ./bin/test_tank_lib)