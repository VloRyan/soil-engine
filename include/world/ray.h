#ifndef SOIL_ENGINE_WORLD_RAY_H
#define SOIL_ENGINE_WORLD_RAY_H
#include <glm/glm.hpp>

#include "stage/scene/viewer/node.h"
namespace soil::world {

struct Ray {
 public:
  glm::vec3 Origin;
  glm::vec3 Direction;
  bool operator==(const Ray& rhs) const {
    return Origin == rhs.Origin && Direction == rhs.Direction;
  }
  bool operator!=(const Ray& rhs) const { return !(rhs == *this); }

  static Ray Calculate(glm::vec2 frag_coord, glm::vec2 resolution,
                       const glm::mat4& viewMatrix,
                       const glm::mat4& projectionMatrix);

 private:
  static glm::vec3 FragCoordToNdc(glm::vec3 frag_coord,
                                  glm::vec2 screen_resolution);
  static glm::vec3 NCDCoordToWorld(glm::vec3 clip_coord,
                                   const glm::mat4& viewMatrix,
                                   const glm::mat4& projectionMatrix);
  static glm::vec3 FragCoordToWorld(glm::vec3 frag_coord,
                                    glm::vec2 screen_resolution,
                                    const glm::mat4& viewMatrix,
                                    const glm::mat4& projectionMatrix);
};
}  // namespace soil::world

#endif
