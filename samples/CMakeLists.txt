cmake_minimum_required(VERSION 3.31.6)
project(soil_engine_samples CXX)
set(CMAKE_CXX_STANDARD 23)

include_directories(src, ${CMAKE_CURRENT_BINARY_DIR})

get_filename_component(ASSET_FILE_PATH assets ABSOLUTE)
configure_file(src/asset.h.in asset.h)

add_executable(${PROJECT_NAME}
        src/main.cc

        src/basic/stage.cc
        src/common/component/shape.cc
        src/common/component/shape_tile.cc
        src/common/component/text_component.cc
        src/common/globals.hpp
        src/common/node/checkbox.cc
        src/common/node/label.cc
        src/common/node/pane.cc
        src/common/node/tab_pane.cc
        src/common/rotation_node.cc
        src/common/stage.cc
        src/gui/menu/item.cc
        src/gui/menu/menu.cc
        src/gui/menu/scrollbar.cc
        src/gui/stage.cc
        src/instancing/shape_instance.cc
        src/instancing/stage.cc
        src/line/line_instance.cc
        src/line/stage.cc
        src/sound/stage.cc
        src/text/node.cc
        src/text/stage.cc
        src/world/stage.cc
        src/common/component/shape_component.hpp
        src/common/component/prefab.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} soil_engine glm::glm plog::plog)
