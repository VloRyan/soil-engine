#include "world/volume/aabb.h"

#include <bits/ranges_algobase.h>

#include <algorithm>

#include "stage/scene/node.h"
#include "world/intersection.h"

namespace soil::world::volume {
AABB::AABB(const glm::vec3 size) : size_(size) {}

AABB::~AABB() = default;
/*
void AABB::UpdateWorldPoints(const glm::vec3& position) {
  glm::vec3 min(std::numeric_limits<float>::max());
  glm::vec3 max(std::numeric_limits<float>::lowest());
  for (const auto point : points_) {
    glm::vec3 worldPoint = point;
    for (auto j = 0; j < 3; j++) {
      min[j] = std::min(min[j], worldPoint[j]);
      max[j] = std::max(max[j], worldPoint[j]);
    }
  }
  minPoint_ = min + position;
  maxPoint_ = max + position;

    worldPoints_[static_cast<int>(Point::TOP_FRONT_LEFT)] =
        glm::vec4(minPoint_.x, maxPoint_.y, maxPoint_.z, 1.0F);
    worldPoints_[static_cast<int>(Point::TOP_BACK_LEFT)] =
        glm::vec4(minPoint_.x, maxPoint_.y, minPoint_.z, 1.0F);
    worldPoints_[static_cast<int>(Point::TOP_BACK_RIGHT)] =
        glm::vec4(maxPoint_.x, maxPoint_.y, minPoint_.z, 1.0F);
    worldPoints_[static_cast<int>(Point::TOP_FRONT_RIGHT)] =
        glm::vec4(maxPoint_.x, maxPoint_.y, maxPoint_.z, 1.0F);

    worldPoints_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)] =
        glm::vec4(minPoint_.x, minPoint_.y, maxPoint_.z, 1.0F);
    worldPoints_[static_cast<int>(Point::BOTTOM_BACK_LEFT)] =
        glm::vec4(minPoint_.x, minPoint_.y, minPoint_.z, 1.0F);
    worldPoints_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)] =
        glm::vec4(maxPoint_.x, minPoint_.y, minPoint_.z, 1.0F);
    worldPoints_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)] =
        glm::vec4(maxPoint_.x, minPoint_.y, maxPoint_.z, 1.0F);
}
*/
/*
void AABB::GetTopPlane(std::array<glm::vec4, 4> points) const {
  for (int i = 0; i < 4; i++) {
    points[i] = worldPoints_[i];
  }
}

void AABB::GetBottomPlane(std::array<glm::vec4, 4> points) const {
  for (uint i = 0; i < 4; i++) {
    points[i] = worldPoints_[4 + i];
  }
}

void AABB::GetAllPoints(std::array<glm::vec4, 8> points) const {
  std::ranges::copy(worldPoints_, points.begin());
}
*/
void AABB::SetSize(const glm::vec3 size) {
  size_ = size;
  /*const auto halfDim = dimension * glm::vec3(0.5F);
  points_[static_cast<int>(Point::TOP_FRONT_LEFT)] =
      glm::vec3(-halfDim.x, halfDim.y, halfDim.z);
  points_[static_cast<int>(Point::TOP_BACK_LEFT)] =
      glm::vec3(-halfDim.x, halfDim.y, -halfDim.z);
  points_[static_cast<int>(Point::TOP_BACK_RIGHT)] =
      glm::vec3(halfDim.x, halfDim.y, -halfDim.z);
  points_[static_cast<int>(Point::TOP_FRONT_RIGHT)] =
      glm::vec3(halfDim.x, halfDim.y, halfDim.z);

  points_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)] =
      glm::vec3(-halfDim.x, -halfDim.y, halfDim.z);
  points_[static_cast<int>(Point::BOTTOM_BACK_LEFT)] =
      glm::vec3(-halfDim.x, -halfDim.y, -halfDim.z);
  points_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)] =
      glm::vec3(halfDim.x, -halfDim.y, -halfDim.z);
  points_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)] =
      glm::vec3(halfDim.x, -halfDim.y, halfDim.z);*/
}

/*void AABB::SetPosition(const glm::vec3& position) {
  UpdateWorldPoints(position);
}

glm::vec3 AABB::GetPosition() const { return maxPoint_ - minPoint_; }*/

/*glm::vec3 AABB::GetPoint(const Point point) const {
  return worldPoints_[static_cast<int>(point)];
}*/
/*
const glm::vec3& AABB::GetMinPoint() const { return minPoint_; }

const glm::vec3& AABB::GetMaxPoint() const { return maxPoint_; }

*/

bool AABB::IsInside(const glm::vec3& at, const glm::vec3& min,
                    const glm::vec3& max) const {
  const auto halfSize = size_ * glm::vec3(0.5F);
  const auto minPoint = at - halfSize;
  const auto maxPoint = at + halfSize;
  for (auto i = 0; i < 3; i++) {
    if (minPoint[i] > max[i] || maxPoint.x < min.x) {
      return false;
    }
  }
  return true;
}

bool AABB::IsInsideXZ(const glm::vec3& at, const glm::vec3& min,
                      const glm::vec3& max) const {
  const auto halfSize = size_ * glm::vec3(0.5F);
  const auto minPoint = at - halfSize;
  const auto maxPoint = at + halfSize;
  if (minPoint.x > max.x || maxPoint.x < min.x) {
    return false;
  }
  if (minPoint.z > max.z || maxPoint.z < min.z) {
    return false;
  }
  return true;
}

bool AABB::IsInside(const glm::vec3& at, const glm::vec2& min,
                    const glm::vec2& max) const {
  const auto halfSize = size_ * glm::vec3(0.5F);
  const auto minPoint = at - halfSize;
  const auto maxPoint = at + halfSize;
  if (minPoint.x > max.x ||  //
      maxPoint.x < min.x ||  //
      minPoint.z > max.y ||  //
      maxPoint.z < min.y) {
    return false;
  }
  return true;
}

bool AABB::Contains(const glm::vec3& at, const glm::vec3& point) const {
  const auto halfSize = size_ * glm::vec3(0.5F);
  const auto minPoint = at - halfSize;
  const auto maxPoint = at + halfSize;
  for (auto i = 0; i < 3; i++) {
    if (point[i] < minPoint[i] || point[i] > maxPoint[i]) {
      return false;
    }
  }
  return true;
}

bool AABB::ContainsXZ(const glm::vec3& at, const glm::vec3& point) const {
  const auto halfSize = size_ * glm::vec3(0.5F);
  const auto minPoint = at - halfSize;
  const auto maxPoint = at + halfSize;
  if (point.x < minPoint.x || point.x > maxPoint.x) {
    return false;
  }
  if (point.z < minPoint.z || point.z > maxPoint.z) {
    return false;
  }
  return true;
}

IntersectionResult AABB::IntersectsRay(const glm::vec3& at,
                                       const glm::vec3& start,
                                       const glm::vec3& dir) const {
  const auto halfSize = size_ * glm::vec3(0.5F);
  const auto minPoint = at - halfSize;
  const auto maxPoint = at + halfSize;
  constexpr auto NUMDIM = 3;
  constexpr auto RIGHT = 0;
  constexpr auto LEFT = 1;
  constexpr auto MIDDLE = 2;

  glm::vec3 hitPoint;

  bool inside = true;
  char quadrant[NUMDIM];
  int i;
  float maxT[NUMDIM];
  float candidatePlane[NUMDIM];

  // Find candidate planes; this loop can be avoided if rays cast all from the
  // eye(assume perspective view)
  for (i = 0; i < NUMDIM; i++) {
    if (start[i] < minPoint[i]) {
      quadrant[i] = LEFT;
      candidatePlane[i] = minPoint[i];
      inside = false;
    } else if (start[i] > maxPoint[i]) {
      quadrant[i] = RIGHT;
      candidatePlane[i] = maxPoint[i];
      inside = false;
    } else {
      quadrant[i] = MIDDLE;
    }
  }

  // Ray origin inside bounding box
  if (inside) {
    return IntersectionResult{.Intersecting = true, .IntersectionPoint = start};
  }

  // Calculate T distances to candidate planes */
  for (i = 0; i < NUMDIM; i++) {
    if (quadrant[i] != MIDDLE && dir[i] != 0.) {
      maxT[i] = (candidatePlane[i] - start[i]) / dir[i];
    } else {
      maxT[i] = -1.;
    }
  }

  // Get largest of the maxT's for final choice of intersection */
  int whichPlane = 0;
  for (i = 1; i < NUMDIM; i++) {
    if (maxT[whichPlane] < maxT[i]) {
      whichPlane = i;
    }
  }

  // Check final candidate actually inside box
  if (maxT[whichPlane] < 0.) {
    return IntersectionResult{.Intersecting = false,
                              .IntersectionPoint = glm::vec3(0, 0, 0)};
  }
  for (i = 0; i < NUMDIM; i++) {
    if (whichPlane != i) {
      hitPoint[i] = start[i] + maxT[whichPlane] * dir[i];
      if (hitPoint[i] < minPoint[i] || hitPoint[i] > maxPoint[i]) {
        return IntersectionResult{.Intersecting = false,
                                  .IntersectionPoint = glm::vec3(0, 0, 0)};
      }
    } else {
      hitPoint[i] = candidatePlane[i];
    }
  }
  return IntersectionResult{.Intersecting = true,
                            .IntersectionPoint = hitPoint};  // ray hits box
}

IntersectionResult AABB::IntersectsRayXZ(const glm::vec3& at,
                                         const glm::vec3& start,
                                         const glm::vec3& dir) const {
  return IntersectsRay(at, {start.x, 0.F, start.z}, {dir.x, 0.F, dir.z});
}

bool AABB::IntersectsCircle(const glm::vec3& at, const glm::vec2& circleCenter,
                            const float radius) const {
  const auto halfSize = size_ * glm::vec3(0.5F);
  const auto minPoint = at - halfSize;
  const auto maxPoint = at + halfSize;
  return Intersection::Test(
      Intersection::Box2d{
          .Min = glm::vec2(minPoint.x, minPoint.z),
          .Max = glm::vec2(maxPoint.x, maxPoint.z),
      },
      Intersection::Circle2d{
          .Center = circleCenter,
          .Radius = radius,
      });
}

std::vector<Line> AABB::GenerateLines(const glm::vec3& at) const {
  std::array<glm::vec3, 8> points{};
  const auto halfSize = size_ * glm::vec3(0.5F);
  points[static_cast<int>(Point::TOP_FRONT_LEFT)] =
      glm::vec3(-halfSize.x, halfSize.y, halfSize.z);
  points[static_cast<int>(Point::TOP_BACK_LEFT)] =
      glm::vec3(-halfSize.x, halfSize.y, -halfSize.z);
  points[static_cast<int>(Point::TOP_BACK_RIGHT)] =
      glm::vec3(halfSize.x, halfSize.y, -halfSize.z);
  points[static_cast<int>(Point::TOP_FRONT_RIGHT)] =
      glm::vec3(halfSize.x, halfSize.y, halfSize.z);

  points[static_cast<int>(Point::BOTTOM_FRONT_LEFT)] =
      glm::vec3(-halfSize.x, -halfSize.y, halfSize.z);
  points[static_cast<int>(Point::BOTTOM_BACK_LEFT)] =
      glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z);
  points[static_cast<int>(Point::BOTTOM_BACK_RIGHT)] =
      glm::vec3(halfSize.x, -halfSize.y, -halfSize.z);
  points[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)] =
      glm::vec3(halfSize.x, -halfSize.y, halfSize.z);
  return {
      {.Start = points[static_cast<int>(Point::TOP_BACK_LEFT)],
       .End = points[static_cast<int>(Point::TOP_BACK_RIGHT)]},
      {.Start = points[static_cast<int>(Point::TOP_BACK_RIGHT)],
       .End = points[static_cast<int>(Point::TOP_FRONT_RIGHT)]},
      {.Start = points[static_cast<int>(Point::TOP_FRONT_RIGHT)],
       .End = points[static_cast<int>(Point::TOP_FRONT_LEFT)]},
      {.Start = points[static_cast<int>(Point::TOP_FRONT_LEFT)],
       .End = points[static_cast<int>(Point::TOP_BACK_LEFT)]},

      {.Start = points[static_cast<int>(Point::BOTTOM_BACK_LEFT)],
       .End = points[static_cast<int>(Point::BOTTOM_BACK_RIGHT)]},
      {.Start = points[static_cast<int>(Point::BOTTOM_BACK_RIGHT)],
       .End = points[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)]},
      {.Start = points[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)],
       .End = points[static_cast<int>(Point::BOTTOM_FRONT_LEFT)]},
      {.Start = points[static_cast<int>(Point::BOTTOM_FRONT_LEFT)],
       .End = points[static_cast<int>(Point::BOTTOM_BACK_LEFT)]},

      {.Start = points[static_cast<int>(Point::BOTTOM_BACK_LEFT)],
       .End = points[static_cast<int>(Point::TOP_BACK_LEFT)]},
      {.Start = points[static_cast<int>(Point::BOTTOM_BACK_RIGHT)],
       .End = points[static_cast<int>(Point::TOP_BACK_RIGHT)]},
      {.Start = points[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)],
       .End = points[static_cast<int>(Point::TOP_FRONT_RIGHT)]},
      {.Start = points[static_cast<int>(Point::BOTTOM_FRONT_LEFT)],
       .End = points[static_cast<int>(Point::TOP_FRONT_LEFT)]},

  };
}
void AABB::GetAABB(glm::vec3& min, glm::vec3& max) const {
  const auto halfSize = size_ * glm::vec3(0.5F);
  min = -halfSize;
  max = halfSize;
}
}  // namespace soil::world::volume
