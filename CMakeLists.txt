cmake_minimum_required(VERSION 3.31.6)
project(soil_engine CXX)

set(CMAKE_CXX_STANDARD 23)

option(BUILD_SAMPLES "Build samples" OFF)

#TODO
add_compile_definitions(DEBUG)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(gl3w REQUIRED)
find_package(DevIL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(glm REQUIRED)
find_package(plog REQUIRED)
find_package(Ogg REQUIRED)
find_package(Vorbis REQUIRED)

include_directories(include)

add_library(${PROJECT_NAME}
        include/event/handler.hpp
        include/event/observable.hpp
        include/file/sequence.hpp
        include/input/constants.hpp
        include/sound/event/event.hpp
        include/stage/scene/component/render/render_component.hpp
        include/util/pointer_cache.hpp
        include/video/render/data/writer.hpp
        include/video/render/state_identifier.hpp
        include/video/render/types.hpp
        src/engine.cc
        src/file/font.cc
        src/file/sprite_sheet.cc
        src/input/event.cc
        src/input/event_map.cc
        src/input/manager.cc
        src/sound/file.cc
        src/sound/ogg.cc
        src/sound/openal/buffer.cc
        src/sound/openal/listener.cc
        src/sound/openal/manager.cc
        src/sound/openal/source.cc
        src/sound/wave.cc
        src/stage/event/component.cc
        src/stage/event/node.cc
        src/stage/event/stage_event.cc
        src/stage/hook/trigger_hook.cc
        src/stage/manager.cc
        src/stage/resources.cc
        src/stage/scene/component/collision_object_component.cc
        src/stage/scene/component/component.cc
        src/stage/scene/component/event/event_component.cc
        src/stage/scene/component/event/hook.cc
        src/stage/scene/component/input/input_component.cc
        src/stage/scene/component/listener_component.cc
        src/stage/scene/component/mesh_component.cc
        src/stage/scene/component/render/mesh_instance_component.cc
        src/stage/scene/component/render/render_state_container_component.cc
        src/stage/scene/component/render/renderable_component.cc
        src/stage/scene/component/render/update_matrices_ubo_component.cc
        src/stage/scene/component/sound.cc
        src/stage/scene/component/text/abstract_text.cc
        src/stage/scene/component/text/parser.cc
        src/stage/scene/component/transform_component.cc
        src/stage/scene/component/update_graph_component.cc
        src/stage/scene/gui/container/base.cc
        src/stage/scene/gui/container/h_box.cc
        src/stage/scene/gui/container/v_box.cc
        src/stage/scene/gui/overlay.cc
        src/stage/scene/gui/rectangle.cc
        src/stage/scene/gui/root.cc
        src/stage/scene/node.cc
        src/stage/scene/scene.cc
        src/stage/scene/text/parser.cc
        src/stage/scene/viewer/node.cc
        src/stage/scene/viewer/ortho.cc
        src/stage/scene/viewer/perspective.cc
        src/stage/scene/volume/frustum_culling.cc
        src/stage/scene/world/world_node.cc
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
        src/video/manager.cc
        src/video/mesh/cache.cc
        src/video/mesh/data.cc
        src/video/mesh/prefab.cc
        src/video/model/letter.cc
        src/video/render/data/buffer_writer.cc
        src/video/render/data/buffer_writer.cc
        src/video/render/mesh_instance.cc
        src/video/render/mesh_instance_pile.cc
        src/video/render/mesh_renderable.cc
        src/video/render/properties.cc
        src/video/render/renderable.cc
        src/video/render/renderable_group_elements.cc
        src/video/render/renderable_group_instances.cc
        src/video/render/state.cc
        src/video/render/state_container.cc
        src/video/shader/cache.cc
        src/video/shader/definition.cc
        src/video/shader/shader.cc
        src/video/shader/stage.cc
        src/video/texture/animation.cc
        src/video/texture/data.cc
        src/video/texture/manager.cc
        src/video/texture/texture.cc
        src/video/vertex/attribute_pointer.cc
        src/video/vertex/vao.cc
        src/window.cc
        src/window_event.cc
        src/world/entity/collision_object.cc
        src/world/entity/object_3d.cc
        src/world/entity/transform_3d.cc
        src/world/intersection.cc
        src/world/ray.cc
        src/world/volume/aabb.cc
        src/world/volume/frustum.cc
        src/world/volume/quad_tree.cc
        src/world/world.cc

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
        include/event/event.hpp
        include/event/handler.hpp
        include/event/observable.hpp
        include/file/font.h
        include/file/sequence.hpp
        include/file/sprite_sheet.h
        include/input/constants.hpp
        include/input/event.h
        include/input/event_map.h
        include/input/manager.h
        include/sound/buffer.h
        include/sound/event/event.hpp
        include/sound/event/source_event.hpp
        include/sound/file.h
        include/sound/listener.h
        include/sound/manager.h
        include/sound/ogg.h
        include/sound/source.h
        include/sound/wave.h
        include/stage/event/component.h
        include/stage/event/game_event.h
        include/stage/event/node.h
        include/stage/event/stage_event.h
        include/stage/hook/event_hook.hpp
        include/stage/hook/trigger_hook.h
        include/stage/interfaces.h
        include/stage/manager.h
        include/stage/resources.h
        include/stage/scene/component/collision_object_component.h
        include/stage/scene/component/component.h
        include/stage/scene/component/event/event_component.h
        include/stage/scene/component/event/hook.h
        include/stage/scene/component/input/input_component.h
        include/stage/scene/component/listener_component.h
        include/stage/scene/component/mesh_component.h
        include/stage/scene/component/render/mesh_instance_component.h
        include/stage/scene/component/render/render_component.hpp
        include/stage/scene/component/render/render_state_container_component.h
        include/stage/scene/component/render/renderable_component.h
        include/stage/scene/component/render/update_matrices_ubo_component.h
        include/stage/scene/component/sound.h
        include/stage/scene/component/text/abstract_text.h
        include/stage/scene/component/text/parser.h
        include/stage/scene/component/transform_component.h
        include/stage/scene/component/update_graph_component.h
        include/stage/scene/gui/container/base.h
        include/stage/scene/gui/container/h_box.h
        include/stage/scene/gui/container/v_box.h
        include/stage/scene/gui/overlay.h
        include/stage/scene/gui/rectangle.h
        include/stage/scene/gui/root.h
        include/stage/scene/node.h
        include/stage/scene/scene.h
        include/stage/scene/text/parser.h
        include/stage/scene/viewer/node.h
        include/stage/scene/viewer/ortho.h
        include/stage/scene/viewer/perspective.h
        include/stage/scene/volume/frustum_culling.h
        include/stage/scene/world/world_node.h
        include/stage/stage.h
        include/util/deque.hpp
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
        include/video/manager.h
        include/video/mesh
        include/video/mesh/cache.h
        include/video/mesh/data.h
        include/video/model/letter.h
        include/video/render/data/buffer_writer.h
        include/video/render/data/writer.hpp
        include/video/render/mesh_instance.h
        include/video/render/mesh_instance_pile.h
        include/video/render/mesh_renderable.h
        include/video/render/properties.h
        include/video/render/renderable.h
        include/video/render/renderable_group.h
        include/video/render/renderable_group_elements.h
        include/video/render/renderable_group_instances.h
        include/video/render/state.h
        include/video/render/state_container.h
        include/video/render/state_identifier.hpp
        include/video/render/types.hpp
        include/video/shader/cache.h
        include/video/shader/definition.h
        include/video/shader/shader.h
        include/video/shader/stage.h
        include/video/texture/animation.h
        include/video/texture/data.h
        include/video/texture/manager.h
        include/video/texture/parameter.h
        include/video/texture/texture.h
        include/video/vertex/attribute_pointer.h
        include/video/vertex/vao.h
        include/video/vertex/vertex.h
        include/window.h
        include/window_event.h
        include/world/entity/collision_object.h
        include/world/entity/object_3d.h
        include/world/entity/transform_3d.h
        include/world/intersection.h
        include/world/ray.h
        include/world/volume/aabb.h
        include/world/volume/container.h
        include/world/volume/container.h
        include/world/volume/frustum.h
        include/world/volume/quad_tree.h
        include/world/volume/quad_tree.h
        include/world/volume/volume.hpp
        include/world/world.h
)

target_link_libraries(${PROJECT_NAME} gl3w::gl3w glfw DevIL::DevIL OpenAL::OpenAL glm::glm plog::plog Ogg::ogg vorbis::vorbis)

if (NOT BUILD_TESTING STREQUAL OFF)
    add_subdirectory(test)
    #add_subdirectory(benchmark)
endif ()


if (BUILD_SAMPLES)
    add_subdirectory(samples)
endif (BUILD_SAMPLES)

install(TARGETS ${PROJECT_NAME} FILE_SET public_headers)
