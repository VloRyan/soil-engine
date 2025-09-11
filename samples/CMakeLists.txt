cmake_minimum_required(VERSION 3.31.6)
project(soil_engine_samples CXX)
set(CMAKE_CXX_STANDARD 23)

include_directories(src, ${CMAKE_CURRENT_BINARY_DIR})

get_filename_component(ASSET_FILE_PATH assets ABSOLUTE)
configure_file(src/asset.h.in asset.h)

add_executable(${PROJECT_NAME} src/main.cc
        src/basic/shader.cc
        src/basic/shader.h
        src/basic/shape.cc
        src/basic/shape.h
        src/basic/stage.cc
        src/basic/stage.h
        src/common/rotation_node.cc
        src/common/rotation_node.h
        src/instancing/shader.cc
        src/instancing/shader.h
        src/instancing/shape_instance.cc
        src/instancing/shape_instance.h
        src/instancing/stage.cc
        src/instancing/stage.h
        src/volume/stage.cc
        src/volume/stage.h

        src/text/stage.cc
        src/text/stage.h
        src/text/node.cc
        src/text/node.h

        src/gui/stage.cc
        src/gui/stage.h
        src/gui/plane.cc
        src/gui/plane.h
        src/gui/label.cc
        src/gui/label.h
        src/gui/component/shape_tile_instance.cc
        src/gui/component/shape_tile_instance.h
        src/gui/component/text.cc
        src/gui/component/text.h
        src/gui/component/shape_tile.cc
        src/gui/component/shape_tile.h
        src/gui/shape_tile_shader.cc
        src/gui/shape_tile_shader.h
        src/gui/shape_tile_instance_shader.cc
        src/gui/shape_tile_instance_shader.h

        src/gui/character_shader.cc
        src/gui/character_shader.h
        src/gui/menu/menu.cc
        src/gui/menu/menu.h
        src/gui/menu/item.cc
        src/gui/menu/item.h
        src/gui/menu/scrollbar.cc
        src/gui/menu/scrollbar.h

        src/line/shader.cc
        src/line/shader.h
        src/line/line_instance.cc
        src/line/line_instance.h
        src/line/stage.cc
        src/line/stage.h
        src/common/stage.cc
        src/common/stage.h
)

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} soil_engine glm::glm plog::plog)
