cmake_minimum_required(VERSION 3.31.6)
project(test_soil_engine CXX)

find_package(GTest REQUIRED CONFIG)

include_directories(. ${CMAKE_CURRENT_BINARY_DIR})

get_filename_component(RESOURCES_FILE_PATH resources ABSOLUTE)
configure_file(resources.h.in resources.h)

add_executable(${PROJECT_NAME}
        file/font_test.cc
        file/sprite_sheet_test.cc
        stage/manager_test.cc
        stage/scene/component/world/collision_object_component_test.cc
        stage/scene/component/component_test.cc
        stage/scene/component/event/event_component_test.cc
        stage/scene/component/event/hook_test.cc
        stage/scene/component/render/drawable_container_component_test.cc
        stage/scene/component/transform_component_test.cc
        stage/scene/component/update_graph_component_test.cc
        stage/scene/gui/container/h_box_test.cc
        stage/scene/gui/container/v_box_test.cc
        stage/scene/gui/rectangle_test.cc
        stage/scene/gui/root_test.cc
        stage/scene/mocks.hpp
        stage/scene/node_test.cc
        stage/scene/scene_test.cc
        stage/stage_test.cc
        util/files_test.cc
        video/buffer/mocks.hpp
        video/render/draw/drawable_container_test.cc
        video/render/draw/drawable_container_test.cc
        video/render/draw/instance_data_buffer_test.cc
        video/render/draw/mocks.hpp
        video/render/forward/forward_render_test.cc
        video/render/mocks.hpp
        video/render/state_identifier_test.cc
        video/shader/mocks.hpp
        world/intersection_test.cc
        world/ray_test.cc
        world/volume/aabb_test.cc
        world/volume/frustum_test.cc
        world/volume/mocks.hpp
        world/volume/quad_tree_perf_test.cc
        world/volume/quad_tree_test.cc
        world/world_test.cc
        video/texture/animation_test.cc
        stage/text/parser_test.cc

)

target_link_libraries(${PROJECT_NAME} soil_engine GTest::gtest GTest::gtest_main)
