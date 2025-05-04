#include "stage/scene/component/aabb.h"
#include <algorithm>
#include <bits/ranges_algobase.h>

#include "stage/scene/node.h"

namespace stage::scene::component {
AABB::AABB(const glm::vec3 dimension, const glm::vec3 scale)
    : points_(), worldPoints_(), scale_(scale), minPoint_(),
      debugDrawer_(nullptr), firstLineId(-1), maxPoint_() {
  contactTypes_.set();
  AABB::SetDimension(dimension);
}

AABB::~AABB() = default;

void AABB::UpdateWorldPoints(const glm::vec3 &position) {
  glm::vec3 min(std::numeric_limits<float>::max());
  glm::vec3 max(std::numeric_limits<float>::lowest());
  for (auto point : points_) {
    glm::vec3 worldPoint = point * scale_;
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

void AABB::UpdateMatrix(const glm::mat4 &matrix) {
  BoundingVolume::UpdateMatrix(matrix);
  UpdateWorldPoints(matrix[3]);
}

void AABB::SetScale(const glm::vec3 scale) {
  if (scale_ == scale) {
    return;
  }
  scale_ = glm::vec4(scale, 1.0F);
  SetDirty();
}

glm::vec3 AABB::GetScale() { return scale_; }

void AABB::SetDimension(const glm::vec3 dimension) {
  const auto halfDim = dimension * glm::vec3(0.5F);
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
      glm::vec3(halfDim.x, -halfDim.y, halfDim.z);
  SetDirty();
}

glm::vec3 AABB::GetPoint(const Point point) const {
  return worldPoints_[static_cast<int>(point)];
}

const glm::vec3 &AABB::GetMinPoint() const { return minPoint_; }

const glm::vec3 &AABB::GetMaxPoint() const { return maxPoint_; }

bool AABB::IsInsideXZ(const glm::vec2 min, const glm::vec2 max) const {
  if (min[0] > minPoint_[0]) {
    return false;
  }
  if (max[0] < maxPoint_[0]) {
    return false;
  }
  if (min[1] > minPoint_[2]) {
    return false;
  }
  if (max[1] < maxPoint_[2]) {
    return false;
  }

  return true;
}

bool AABB::ContainsXZ(const glm::vec2 point) const {
  if (point[0] < minPoint_[0]) {
    return false;
  }
  if (point[0] > maxPoint_[0]) {
    return false;
  }
  if (point[1] < minPoint_[2]) {
    return false;
  }
  if (point[1] > maxPoint_[2]) {
    return false;
  }
  return true;
}

IntersectionResult AABB::IntersectsRay2d(const glm::vec2 start,
                                         const glm::vec2 dir) const {
  const auto start3d = glm::vec3(start.x, 0, start.y);
  const auto dir3d = glm::vec3(dir.x, 0, dir.y);
  return IntersectsRay(start3d, dir3d);
}

IntersectionResult AABB::IntersectsRay(const glm::vec3 start,
                                       const glm::vec3 dir) const {
  constexpr auto NUMDIM = 3;
  constexpr auto RIGHT = 0;
  constexpr auto LEFT = 1;
  constexpr auto MIDDLE = 2;

  glm::vec3 hitPoint;

  bool inside = true;
  char quadrant[NUMDIM];
  int i;
  double maxT[NUMDIM];
  double candidatePlane[NUMDIM];

  /* Find candidate planes; this loop can be avoided if
     rays cast all from the eye(assume perspective view) */
  for (i = 0; i < NUMDIM; i++)
    if (start[i] < minPoint_[i]) {
      quadrant[i] = LEFT;
      candidatePlane[i] = minPoint_[i];
      inside = false;
    } else if (start[i] > maxPoint_[i]) {
      quadrant[i] = RIGHT;
      candidatePlane[i] = maxPoint_[i];
      inside = false;
    } else {
      quadrant[i] = MIDDLE;
    }

  /* Ray origin inside bounding box */
  if (inside) {
    return IntersectionResult{.Intersecting = true, .IntersectionPoint = start};
  }

  /* Calculate T distances to candidate planes */
  for (i = 0; i < NUMDIM; i++) {
    if (quadrant[i] != MIDDLE && dir[i] != 0.) {
      maxT[i] = (candidatePlane[i] - start[i]) / dir[i];
    } else {
      maxT[i] = -1.;
    }
  }

  /* Get largest of the maxT's for final choice of intersection */
  int whichPlane = 0;
  for (i = 1; i < NUMDIM; i++) {
    if (maxT[whichPlane] < maxT[i])
      whichPlane = i;
  }

  /* Check final candidate actually inside box */
  if (maxT[whichPlane] < 0.) {
    return IntersectionResult{.Intersecting = false,
                              .IntersectionPoint = glm::vec3(0, 0, 0)};
    ;
  }
  for (i = 0; i < NUMDIM; i++) {
    if (whichPlane != i) {
      hitPoint[i] = start[i] + maxT[whichPlane] * dir[i];
      if (hitPoint[i] < minPoint_[i] || hitPoint[i] > maxPoint_[i]) {
        return IntersectionResult{.Intersecting = false,
                                  .IntersectionPoint = glm::vec3(0, 0, 0)};
        ;
      }
    } else {
      hitPoint[i] = candidatePlane[i];
    }
  }
  return IntersectionResult{.Intersecting = true,
                            .IntersectionPoint = hitPoint}; /* ray hits box */
}

void AABB::DrawDebugLines(IDebugDrawer *debugDrawer) {
  debugDrawer->StartDraw();
  // FRONT
  debugDrawer->Line(points_[static_cast<int>(Point::TOP_FRONT_LEFT)],
                    points_[static_cast<int>(Point::TOP_FRONT_RIGHT)],
                    glm::vec4(1.0F));

  debugDrawer->Line(points_[static_cast<int>(Point::TOP_FRONT_RIGHT)],
                    points_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)],
                    glm::vec4(1.0F));

  debugDrawer->Line(points_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)],
                    points_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)],
                    glm::vec4(1.0F));

  debugDrawer->Line(points_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)],
                    points_[static_cast<int>(Point::TOP_FRONT_LEFT)],
                    glm::vec4(1.0F));
  // BACK
  debugDrawer->Line(points_[static_cast<int>(Point::TOP_BACK_LEFT)],
                    points_[static_cast<int>(Point::TOP_BACK_RIGHT)],
                    glm::vec4(1.0F));

  debugDrawer->Line(points_[static_cast<int>(Point::TOP_BACK_RIGHT)],
                    points_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)],
                    glm::vec4(1.0F));

  debugDrawer->Line(points_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)],
                    points_[static_cast<int>(Point::BOTTOM_BACK_LEFT)],
                    glm::vec4(1.0F));

  debugDrawer->Line(points_[static_cast<int>(Point::BOTTOM_BACK_LEFT)],
                    points_[static_cast<int>(Point::TOP_BACK_LEFT)],
                    glm::vec4(1.0F));
  // EDGES
  debugDrawer->Line(points_[static_cast<int>(Point::TOP_FRONT_LEFT)],
                    points_[static_cast<int>(Point::TOP_BACK_LEFT)],
                    glm::vec4(1.0F));
  debugDrawer->Line(points_[static_cast<int>(Point::TOP_FRONT_RIGHT)],
                    points_[static_cast<int>(Point::TOP_BACK_RIGHT)],
                    glm::vec4(1.0F));

  debugDrawer->Line(points_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)],
                    points_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)],
                    glm::vec4(1.0F));

  debugDrawer->Line(points_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)],
                    points_[static_cast<int>(Point::BOTTOM_BACK_LEFT)],
                    glm::vec4(1.0F));

  debugDrawer->Line(points_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)],
                    points_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)],
                    glm::vec4(1.0F));
  debugDrawer->EndDraw();
}

void AABB::SetParent(Node *parent) {
  const auto change = GetParent() != parent;
  BoundingVolume::SetParent(parent);
  if (change && GetParent() != nullptr) {
    UpdateWorldPoints(GetParent()->GetWorldPosition());
  }
}

bool AABB::IsContactType(ContactType contactType) const {
  return contactTypes_[static_cast<int>(contactType)];
}

void AABB::SetContactType(ContactType type, const bool value) {
  contactTypes_[static_cast<int>(type)] = value;
}
} // namespace stage::scene::component
