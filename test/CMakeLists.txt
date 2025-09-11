cmake_minimum_required(VERSION 3.31.6)
project(test_soil_engine CXX)

find_package(GTest REQUIRED CONFIG)

include_directories(. ${CMAKE_CURRENT_BINARY_DIR})

get_filename_component(RESOURCES_FILE_PATH resources ABSOLUTE)
configure_file(resources.h.in resources.h)

add_executable(${PROJECT_NAME}
        stage/scene/mocks.hpp
        stage/scene/node_test.cc
        stage/scene/scene_test.cc
        video/render/instance/buffer_test.cc
        video/render/instance/test_buffer_object.cc
        video/render/instance/test_buffer_object.h
        video/render/instance/test_instance.cc
        video/render/instance/test_instance.h
        world/volume/aabb_test.cc
        world/volume/frustum_test.cc
        world/volume/quad_tree_perf_test.cc
        world/volume/quad_tree_test.cc
        stage/scene/component/component_test.cc
        file/sprite_sheet_test.cc
        util/files_test.cc
        video/render/step/sort_test.cc
        file/font_test.cc
        stage/scene/gui/rectangle_test.cc
        stage/stage_test.cc
        stage/manager_test.cc
        stage/scene/gui/container/v_box_test.cc
        stage/scene/gui/container/h_box_test.cc
        stage/scene/gui/root_test.cc
)

target_link_libraries(${PROJECT_NAME} soil_engine GTest::gtest GTest::gtest_main)
