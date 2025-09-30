#ifndef SOIL_STAGE_SCENE_RENDER_TYPE_H
#define SOIL_STAGE_SCENE_RENDER_TYPE_H
#include <cstdint>

namespace soil::stage::scene::render {
enum class Type : std::uint8_t { Plain, Instancing, COUNT };
}
#endif  // SOIL_STAGE_SCENE_RENDER_TYPE_H
