cmake_minimum_required(VERSION 3.15)
project(pkg_test_soil_engine CXX)
set(CMAKE_CXX_STANDARD 23)
find_package(soil_engine CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/init.cpp)
target_link_libraries(${PROJECT_NAME} soil_engine::soil_engine)
