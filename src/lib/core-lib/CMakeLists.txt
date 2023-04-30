set(core_lib_dir .)
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
include_directories(include)
add_library(core-lib ${core_lib_sources})


set(core_lib_test_dir ${core_lib_dir}/test)
set(core_lib_test_sources
        ${core_lib_test_dir}/test_simpleObserver.cpp
        ${core_lib_test_dir}/test_eventObserver.cpp
        ${core_lib_test_dir}/test_eventQueue.cpp .
        ${core_lib_test_dir}/test_event.cpp)

add_executable(test_core_lib ${core_lib_test_sources})
target_link_libraries(test_core_lib PRIVATE core-lib Catch2::Catch2WithMain)