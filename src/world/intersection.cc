#include "../../include/world/intersection.h"

#include <glm/glm.hpp>

namespace soil::world {
bool Intersection::Test(const Box2d& box, const Circle2d& circle) {
  const auto halfDim = (box.Max - box.Min) * glm::vec2(0.5F);
  const auto center = box.Min + halfDim;
  const auto dist = glm::abs(circle.Center - center);
  if (dist.x <= circle.Radius && dist.y <= circle.Radius) {
    return true;
  }
  const auto dir = glm::normalize(center - circle.Center);
  const auto maxPoint = dir * circle.Radius + circle.Center;
  return Test(box, maxPoint);
}

bool Intersection::Test(const Box2d& boxA, const Box2d& boxB) {
  if (boxA.Min.x > boxB.Min.x && boxA.Max.x < boxB.Max.x ||  //
      boxA.Min.y > boxB.Min.y && boxA.Max.y < boxB.Max.y) {
    return false;
  }

  return true;
}

bool Intersection::Test(const Box2d& box, const glm::vec2& point) {
  if (point.x < box.Min.x ||  //
      point.x > box.Max.x ||  //
      point.y < box.Min.y ||  //
      point.y > box.Max.y) {
    return false;
  }
  return true;
}
}  // namespace soil::world
