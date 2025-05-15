cmake_minimum_required(VERSION 3.31.6)
project(soil_engine_samples CXX)
set(CMAKE_CXX_STANDARD 23)

include_directories(src)

add_executable(${PROJECT_NAME} src/main.cc
        src/basic/shader.cc
        src/basic/shader.h
        src/basic/shape.cc
        src/basic/shape.h
        src/basic/stage.cc
        src/basic/stage.h
        src/common/rotation_node.cc
        src/common/rotation_node.h
        src/common/statistic_scene.cc
        src/common/statistic_scene.h
        src/instancing/shader.cc
        src/instancing/shader.h
        src/instancing/shape_instance.cc
        src/instancing/shape_instance.h
        src/instancing/stage.cc
        src/instancing/stage.h
)

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} soil_engine glm::glm plog::plog)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/Shader
        ${CMAKE_CURRENT_BINARY_DIR}/Shader)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/Textures
        ${CMAKE_CURRENT_BINARY_DIR}/Textures)
