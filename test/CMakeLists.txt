cmake_minimum_required(VERSION 3.31.6)
project(test_soil_engine CXX)

find_package(GTest REQUIRED CONFIG)

add_executable(${PROJECT_NAME}
        stage/scene/space/quad_tree_test.cc
        stage/scene/node_test.cc
        world/collision/frustum_test.cc
)
target_link_libraries(${PROJECT_NAME} soil_engine GTest::gtest GTest::gtest_main)