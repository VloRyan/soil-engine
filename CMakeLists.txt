cmake_minimum_required(VERSION 3.31.6)
project(soil_engine CXX)

set(CMAKE_CXX_STANDARD 23)

#TODO
add_compile_definitions(DEBUG)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(gl3w REQUIRED)
find_package(DevIL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(glm REQUIRED)
find_package(plog REQUIRED)

include_directories(include)

add_library(${PROJECT_NAME}
        include/event/handler.hpp
        include/event/observable.hpp
        include/input/constants.hpp
        include/sound/buffer.h
        include/sound/listener.h
        include/sound/manager.h
        include/sound/source.h
        include/stage/scene/render/feature.hpp
        include/stage/scene/render/type.hpp
        include/util/pointer_cache.hpp
        include/video/render/step/context.hpp
        src/engine.cc
        src/event/event.cc
        src/file/font.cc
        src/id.cc
        src/input/event.cc
        src/input/event_map.cc
        src/input/manager.cc
        src/input/mouse/picker.cc
        src/log.cc
        src/sound/file.cc
        src/sound/openal/buffer.cc
        src/sound/openal/listener.cc
        src/sound/openal/manager.cc
        src/sound/openal/source.cc
        src/sound/wave.cc
        src/stage/event/component.cc
        src/stage/event/node.cc
        src/stage/manager.cc
        src/stage/resources.cc
        src/stage/scene/component/aabb.cc
        src/stage/scene/component/bounding_volume.cc
        src/stage/scene/component/component.cc
        src/stage/scene/component/instance_data.cc
        src/stage/scene/component/listener.cc
        src/stage/scene/component/mesh_component.cc
        src/stage/scene/component/sound.cc
        src/stage/scene/grid_scene.cc
        src/stage/scene/input.cc
        src/stage/scene/node.cc
        src/stage/scene/render/plain.cc
        src/stage/scene/render/instancing.cc
        src/stage/scene/scene.cc
        src/stage/scene/space/quad_tree.cc
        src/stage/scene/viewer/node.cc
        src/stage/scene/viewer/ortho.cc
        src/stage/scene/viewer/perspective.cc
        src/stage/stage.cc
        src/util/files.cc
        src/util/strings.cc
        src/util/strings.cc
        src/video/adapter.cc
        src/video/buffer/cursor.cc
        src/video/buffer/ebo.cc
        src/video/buffer/fb.cc
        src/video/buffer/object.cc
        src/video/buffer/ubo.cc
        src/video/buffer/vbo.cc
        src/video/event.cc
        src/video/manager.cc
        src/video/mesh/cache.cc
        src/video/mesh/data.cc
        src/video/mesh/prefab.cc
        src/video/model/letter.cc
        src/video/render/batch.cc
        src/video/render/instance/buffer.cc
        src/video/render/instance/sorted_buffer.cc
        src/video/render/pipeline.cc
        src/video/render/properties.cc
        src/video/render/quad.cc
        src/video/render/state.cc
        src/video/render/step/base.cc
        src/video/render/step/bind_buffer.cc
        src/video/render/step/render.cc
        src/video/render/vertex_array.cc
        src/video/shader/cache.cc
        src/video/shader/definition.cc
        src/video/shader/shader.cc
        src/video/shader/stage.cc
        src/video/texture/data.cc
        src/video/texture/manager.cc
        src/video/texture/texture.cc
        src/video/vertex/attribute_pointer.cc
        src/video/vertex/vao.cc
        src/video/vertex/vao_creator.cc
        src/window.cc
        src/window_event.cc
        src/world/collision/frustum.cc
        src/world/entity/object_3d.cc
        src/video/render/container.cc
        include/video/render/container.h
        src/video/render/renderable.cc
        src/stage/scene/component/visual_component.cc
        src/video/render/instance/instance.cc

)
include_directories(include)
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_sources(
        ${PROJECT_NAME}
        PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS include
        FILES
        include/base.h
        include/engine.h
        include/event/event.h
        include/event/handler.hpp
        include/event/observable.hpp
        include/exception.h
        include/file/font.h
        include/id.h
        include/input/constants.hpp
        include/input/event.h
        include/input/event_map.h
        include/input/manager.h
        include/input/mouse
        include/input/mouse/picker.h
        include/log.h
        include/sound/buffer.h
        include/sound/file.h
        include/sound/listener.h
        include/sound/manager.h
        include/sound/source.h
        include/sound/wave.h
        include/stage/event/component.h
        include/stage/event/node.h
        include/stage/manager.h
        include/stage/resources.h
        include/stage/scene/component/aabb.h
        include/stage/scene/component/bounding_volume.h
        include/stage/scene/component/component.h
        include/stage/scene/component/instance_data.h
        include/stage/scene/component/listener.h
        include/stage/scene/component/mesh_component.h
        include/stage/scene/component/sound.h
        include/stage/scene/component/visual_component.h
        include/stage/scene/debug_drawer.h
        include/stage/scene/grid_scene.h
        include/stage/scene/input.h
        include/stage/scene/node.h
        include/stage/scene/render/feature.hpp
        include/stage/scene/render/instancing.h
        include/stage/scene/render/type.hpp
        include/stage/scene/render/plain.h
        include/stage/scene/scene.h
        include/stage/scene/space/quad_tree.h
        include/stage/scene/viewer/node.h
        include/stage/scene/viewer/ortho.h
        include/stage/scene/viewer/perspective.h
        include/stage/stage.h
        include/util/files.h
        include/util/pointer_cache.hpp
        include/util/strings.h
        include/video/adapter.h
        include/video/buffer/cursor.h
        include/video/buffer/ebo.h
        include/video/buffer/fb.h
        include/video/buffer/object.h
        include/video/buffer/ubo.h
        include/video/buffer/vbo.h
        include/video/event.h
        include/video/manager.h
        include/video/mesh
        include/video/mesh/cache.h
        include/video/mesh/data.h
        include/video/model/letter.h
        include/video/render/batch.h
        include/video/render/forward_rendering.h
        include/video/render/instance/buffer.h
        include/video/render/instance/instance.h
        include/video/render/instance/sorted_buffer.h
        include/video/render/pipeline.h
        include/video/render/container.h
        include/video/render/properties.h
        include/video/render/quad.h
        include/video/render/renderable.h
        include/video/render/state.h
        include/video/render/step/base.h
        include/video/render/step/bind_buffer.h
        include/video/render/step/context.hpp
        include/video/render/step/render.h
        include/video/render/technique.h
        include/video/render/vertex_array.h
        include/video/shader/cache.h
        include/video/shader/definition.h
        include/video/shader/instance_shader.h
        include/video/shader/shader.h
        include/video/shader/stage.h
        include/video/texture/data.h
        include/video/texture/manager.h
        include/video/texture/parameter.h
        include/video/texture/texture.h
        include/video/vertex/attribute_pointer.h
        include/video/vertex/vao.h
        include/video/vertex/vao_creator.h
        include/video/vertex/vertex.h
        include/video/viewer.h
        include/window.h
        include/window_event.h
        include/world/collision/frustum.h
        include/world/entity/object_3d.h
)

target_link_libraries(${PROJECT_NAME} gl3w::gl3w glfw DevIL::DevIL OpenAL::OpenAL glm::glm plog::plog)

if (NOT BUILD_TESTING STREQUAL OFF)
    add_subdirectory(test)
endif ()

install(TARGETS ${PROJECT_NAME} FILE_SET public_headers)

