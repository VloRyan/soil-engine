cmake_minimum_required(VERSION 3.31.6)
project(soil_engine CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(gl3w REQUIRED)
find_package(DevIL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(glm REQUIRED)
find_package(plog REQUIRED)

include_directories(include)

add_library(${PROJECT_NAME}
        src/engine.cc
        src/video/manager.cc
        src/window.cc
        src/exception.cc
        src/log.cc
        src/input/manager.cc
        src/event/event.cc
        src/input/event.cc
        src/video/adapter.cc
        src/util/strings.cc
        src/input/event_map.cc
        src/stage/manager.cc
        src/frame_limiter.cc
        src/stage/event/node.cc
        src/stage/scene/viewer/fps_camera.cc
        src/stage/scene/viewer/node.cc
        src/video/event.cc
        src/video/buffer/object.cc
        src/video/buffer/vbo.cc
        src/video/buffer/frame_buffer.cc
        src/video/buffer/uniform_buffer_object.cc
        src/video/buffer/cursor.cc
        src/video/texture/texture.cc
        src/video/texture/data.cc
        src/util/files.cc
        src/video/texture/manager.cc
        src/video/shader/definition.cc
        src/video/shader/stage.cc
        src/video/vertex/vertex.cc
        src/video/vertex/vao.cc
        src/video/vertex/attribute_pointer.cc
        src/video/buffer/ebo.cc
        src/video/render/instance/instance.cc
        src/video/render/properties.cc
        src/video/model/material.cc
        src/video/render/pipeline.cc
        src/video/render/forward_rendering.cc
        src/video/render/processing/abstract_processing.cc
        src/video/shader/shader.cc
        src/video/render/processing/render_step.cc
        src/video/render/processing/context.cc
        src/util/strings.cc
        src/video/model/letter.cc
        src/file/font.cc
        src/stage/scene/component/component.cc
        src/world/entity/object_3d.cc
        src/stage/scene/viewer/ortho.cc
        src/video/shader/cache.cc
        src/video/mesh/cache.cc
        src/video/mesh/mesh.cc
        src/video/render/technique.cc
        src/video/model/animation.cc
        src/video/model/animation_node.cc
        src/id.cc
        src/world/collision/frustum.cc
        src/stage/scene/space/quad_tree.cc
        src/stage/scene/component/aabb.cc
        src/window_event.cc
        src/stage/scene/component/bounding_volume.cc
        src/input/mouse/picker.cc
        src/stage/scene/viewer/perspective.cc
        src/util/ini_file.cc
        src/video/render/state.cc
        src/sound/manager.cc
        src/sound/listener.cc
        src/sound/buffer.cc
        src/sound/file.cc
        src/sound/source.cc
        src/stage/scene/component/listener.cc
        src/stage/scene/component/sound.cc
        src/sound/wave.cc
        src/stage/event/component.cc
        src/video/render/instance/buffer.cc
        src/video/render/batch.cc
        src/video/vertex/vao_creator.cc
        src/stage/scene/scene.cc
        src/stage/scene/node.cc
        src/stage/scene/input.cc
        src/stage/scene/component/renderable.cc
        src/stage/scene/component/renderable_instance.cc
        src/video/render/instance/sorted_buffer.cc
        src/stage/stage.cc
        src/stage/scene/grid_scene.cc
        src/video/render/quad.cc

        include/event/observable.hpp
        include/util/pointer_cache.hpp
        include/event/handler.hpp
        include/input/constants.hpp
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
        include/event
        include/event/handler.hpp
        include/event/observable.hpp
        include/event/event.h
        include/util
        include/util/pointer_cache.hpp
        include/util/ini_file.h
        include/util/files.h
        include/util/strings.h
        include/video
        include/video/buffer
        include/video/buffer/uniform_buffer_object.h
        include/video/buffer/ebo.h
        include/video/buffer/cursor.h
        include/video/buffer/object.h
        include/video/buffer/frame_buffer.h
        include/video/buffer/vbo.h
        include/video/model
        include/video/model/animation_node.h
        include/video/model/material.h
        include/video/model/animation.h
        include/video/model/letter.h
        include/video/mesh
        include/video/mesh/cache.h
        include/video/mesh/mesh.h
        include/video/render
        include/video/render/pipeline.h
        include/video/render/renderable.h
        include/video/render/quad.h
        include/video/render/technique.h
        include/video/render/batch.h
        include/video/render/properties.h
        include/video/render/forward_rendering.h
        include/video/render/instance
        include/video/render/instance/instance.h
        include/video/render/instance/buffer.h
        include/video/render/instance/sorted_buffer.h
        include/video/render/processing
        include/video/render/processing/render_step.h
        include/video/render/processing/abstract_processing.h
        include/video/render/processing/context.h
        include/video/render/state.h
        include/video/vertex
        include/video/vertex/attribute_pointer.h
        include/video/vertex/vao_creator.h
        include/video/vertex/vertex.h
        include/video/vertex/vao.h
        include/video/adapter.h
        include/video/manager.h
        include/video/texture
        include/video/texture/parameter.h
        include/video/texture/data.h
        include/video/texture/manager.h
        include/video/texture/texture.h
        include/video/viewer.h
        include/video/shader
        include/video/shader/cache.h
        include/video/shader/instance_shader.h
        include/video/shader/stage.h
        include/video/shader/shader.h
        include/video/shader/definition.h
        include/video/event.h
        include/file
        include/file/font.h
        include/sound
        include/sound/file.h
        include/sound/manager.h
        include/sound/wave.h
        include/sound/source.h
        include/sound/buffer.h
        include/sound/listener.h
        include/frame_limiter.h
        include/world
        include/world/entity
        include/world/entity/object_3d.h
        include/world/collision
        include/world/collision/frustum.h
        include/exception.h
        include/engine.h
        include/window_event.h
        include/window.h
        include/id.h
        include/stage
        include/stage/scene
        include/stage/scene/scene.h
        include/stage/scene/space
        include/stage/scene/space/quad_tree.h
        include/stage/scene/debug_drawer.h
        include/stage/scene/grid_scene.h
        include/stage/scene/node.h
        include/stage/scene/viewer
        include/stage/scene/viewer/fps_camera.h
        include/stage/scene/viewer/ortho.h
        include/stage/scene/viewer/node.h
        include/stage/scene/viewer/perspective.h
        include/stage/scene/input.h
        include/stage/scene/component
        include/stage/scene/component/bounding_volume.h
        include/stage/scene/component/sound.h
        include/stage/scene/component/renderable_instance.h
        include/stage/scene/component/renderable.h
        include/stage/scene/component/aabb.h
        include/stage/scene/component/component.h
        include/stage/scene/component/listener.h
        include/stage/event
        include/stage/event/node.h
        include/stage/event/component.h
        include/stage/stage.h
        include/stage/resources.h
        include/stage/manager.h
        include/log.h
        include/input
        include/input/event_map.h
        include/input/constants.hpp
        include/input/mouse
        include/input/mouse/picker.h
        include/input/manager.h
        include/input/event.h
)

target_link_libraries(${PROJECT_NAME} gl3w::gl3w glfw DevIL::DevIL OpenAL::OpenAL glm::glm plog::plog)
install(TARGETS ${PROJECT_NAME} FILE_SET public_headers)

