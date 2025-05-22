cmake_minimum_required(VERSION 3.31.6)
project(test_soil_engine CXX)

find_package(GTest REQUIRED CONFIG)

add_executable(${PROJECT_NAME}
        world/volume/quad_tree_test.cc
        stage/scene/node_test.cc
        world/volume/frustum_test.cc
        video/render/instance/buffer_test.cc
        video/render/instance/test_buffer_object.cc
        video/render/instance/test_buffer_object.h
        video/render/instance/test_buffer_object.h
        video/render/instance/test_instance.cc
        video/render/instance/test_instance.h
        stage/scene/scene_test.cc
)
target_link_libraries(${PROJECT_NAME} soil_engine GTest::gtest GTest::gtest_main)
