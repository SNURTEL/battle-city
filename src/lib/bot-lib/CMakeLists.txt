set(bot_lib_dir .)
set(bot_lib_sources
        ${bot_lib_dir}/Bot.cpp
        ${bot_lib_dir}/BotController.cpp
        )

add_library(bot-lib ${bot_lib_sources})
target_link_libraries(bot-lib PRIVATE core-lib tank-lib)

set(bot_lib_test_dir ${bot_lib_dir}/test)
set(bot_lib_test_sources
        ${bot_lib_test_dir}/test_bot.cpp
        ${bot_lib_test_dir}/test_botController.cpp)

add_executable(test_bot_lib ${bot_lib_test_sources})
target_link_libraries(test_bot_lib PRIVATE tank-lib bot-lib Catch2::Catch2WithMain)

enable_testing()
add_test(BOT_LIB_TEST ./bin/test_bot_lib)