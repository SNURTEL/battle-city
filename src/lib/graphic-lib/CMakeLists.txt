set(graphic_lib_dir .)
set(graphic_lib_sources
        ${graphic_lib_dir}/Window.cpp
        ${graphic_lib_dir}/TanksGraphic.cpp
        ${graphic_lib_dir}/FrameGraphic.cpp
        ${graphic_lib_dir}/ActiveStateGraphic.cpp
        ${graphic_lib_dir}/AbstractWindow.cpp
        ${graphic_lib_dir}/BoardGraphic.cpp
        ${graphic_lib_dir}/BulletsGraphic.cpp
        ${graphic_lib_dir}/TilesGraphic.cpp
        ${graphic_lib_dir}/BareFrameGraphic.cpp
        ${graphic_lib_dir}/GraphicEventHandler.cpp
        ${graphic_lib_dir}/GameInfoGraphic.cpp
        ${graphic_lib_dir}/TextureLoadingError.cpp
        ${graphic_lib_dir}/StaticGraphic.cpp
        ${graphic_lib_dir}/MenuStateGraphic.cpp
        ${graphic_lib_dir}/EagleGraphic.cpp
        )

add_library(graphic-lib ${graphic_lib_sources})
target_link_libraries(graphic-lib PRIVATE board-lib game-lib tank-lib core-lib bot-lib)

set(graphic_lib_test_dir ${graphic_lib_dir}/test)
set(graphic_lib_test_sources
        ${graphic_lib_test_dir}/test_render.cpp
        ${graphic_lib_test_dir}/test_graphicEventHandler.cpp)

add_executable(test_graphic_lib ${graphic_lib_test_sources})
target_link_libraries(test_graphic_lib PRIVATE graphic-lib core-lib Catch2::Catch2WithMain)