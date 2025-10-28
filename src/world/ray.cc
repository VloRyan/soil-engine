#include "world/ray.h"

namespace soil::world {

Ray Ray::Calculate(glm::vec2 frag_coord, glm::vec2 resolution,
                   const glm::mat4 &viewMatrix,
                   const glm::mat4 &projectionMatrix) {
  auto frustum_near = FragCoordToWorld(glm::vec3(frag_coord, 0.0), resolution,
                                       viewMatrix, projectionMatrix);
  auto frustum_far = FragCoordToWorld(glm::vec3(frag_coord, 1.0), resolution,
                                      viewMatrix, projectionMatrix);
  auto vector = frustum_far - frustum_near;
  auto direction = glm::normalize(vector);
  return Ray{
      .Origin = frustum_near,
      .Direction = direction,
  };
}

glm::vec3 Ray::FragCoordToNdc(glm::vec3 frag_coord,
                              glm::vec2 screen_resolution) {
  auto invert_y = glm::vec2(1.0, -1.0);
  return {((glm::vec2(2.F) * glm::vec2(frag_coord.x, frag_coord.y) /
                screen_resolution -
            1.F) *
           invert_y),
          frag_coord.z};
}
glm::vec3 Ray::NCDCoordToWorld(glm::vec3 clip_coord,
                               const glm::mat4 &viewMatrix,
                               const glm::mat4 &projectionMatrix) {
  auto p =
      glm::inverse(projectionMatrix * viewMatrix) * glm::vec4(clip_coord, 1.0);
  return glm ::vec3(p) / p.w;
}

glm::vec3 Ray::FragCoordToWorld(glm::vec3 frag_coord,
                                glm::vec2 screen_resolution,
                                const glm::mat4 &viewMatrix,
                                const glm::mat4 &projectionMatrix) {
  // Convert fragment coordinates to clip
  auto clip_coord = FragCoordToNdc(frag_coord, screen_resolution);
  // Transform point to world space
  auto world_point =
      NCDCoordToWorld(glm::vec3(clip_coord), viewMatrix, projectionMatrix);
  return {world_point};
}
/*
glm::vec3 Ray::Calculate(const glm::ivec2 &mousePos,
                         const glm::ivec2 &windowSize,
                         const glm::mat4 &viewMatrix,
                         const glm::mat4 &projectionMatrix) {
  const auto nds = getNormalisedDeviceCoordinates(mousePos, windowSize);
  const auto clipCoords = glm::vec4(nds, -1.0F, 1.0F);
  const auto eyeCoords = toEyeCoords(clipCoords, projectionMatrix);
  return toWorldCoords(eyeCoords, viewMatrix);
}

glm::vec2 Ray::getNormalisedDeviceCoordinates(const glm::ivec2 &mousePos,
                                              const glm::ivec2 &windowSize) {
  glm::vec2 nds;
  nds.x =
      2.F * static_cast<float>(mousePos.x) / static_cast<float>(windowSize.x) -
      1.F;
  nds.y = 1.F - 2.F * static_cast<float>(mousePos.y) /
                    static_cast<float>(windowSize.y);
  return nds;
}

glm::vec4 Ray::toEyeCoords(const glm::vec4 &clipCoords,
                           const glm::mat4 &projMatrix) {
  const auto invertedProjection = glm::inverse(projMatrix);
  auto eyeCoords = invertedProjection * clipCoords;
  return {eyeCoords.x, eyeCoords.y, -1.F, 0.F};
}

glm::vec3 Ray::toWorldCoords(const glm::vec4 &eyeCoords,
                             const glm::mat4 &viewMatrix) {
  const auto invertedView = glm::inverse(viewMatrix);
  const auto rayWorld = invertedView * eyeCoords;
  return glm::normalize(rayWorld);
}*/

}  // namespace soil::world
