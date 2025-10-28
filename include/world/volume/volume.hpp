#ifndef SOIL_WORLD_VOLUME_H
#define SOIL_WORLD_VOLUME_H

#include "glm/glm.hpp"
#include "vector"

namespace soil::world::volume {
struct IntersectionResult {
  bool Intersecting = false;
  glm::vec3 IntersectionPoint;
};

struct Line {
  glm::vec3 Start;
  glm::vec3 End;

  friend bool operator==(const Line& lhs, const Line& rhs) {
    return lhs.Start == rhs.Start && lhs.End == rhs.End;
  }

  friend bool operator!=(const Line& lhs, const Line& rhs) {
    return !(lhs == rhs);
  }
};

class Volume {
 public:
  virtual ~Volume() = default;

  [[nodiscard]] virtual bool IsInside(const glm::vec3& at, const glm::vec3& min,
                                      const glm::vec3& max) const = 0;

  [[nodiscard]] virtual bool IsInsideXZ(const glm::vec3& at,
                                        const glm::vec3& min,
                                        const glm::vec3& max) const = 0;

  [[nodiscard]] virtual bool IsInside(const glm::vec3& at, const glm::vec2& min,
                                      const glm::vec2& max) const = 0;

  [[nodiscard]] virtual bool Contains(const glm::vec3& at,
                                      const glm::vec3& point) const = 0;

  [[nodiscard]] virtual bool ContainsXZ(const glm::vec3& at,
                                        const glm::vec3& point) const = 0;

  [[nodiscard]] virtual IntersectionResult IntersectsRay(
      const glm::vec3& at, const glm::vec3& start,
      const glm::vec3& dir) const = 0;

  [[nodiscard]] virtual IntersectionResult IntersectsRayXZ(
      const glm::vec3& at, const glm::vec3& start,
      const glm::vec3& dir) const = 0;

  [[nodiscard]] virtual bool IntersectsCircle(const glm::vec3& at,
                                              const glm::vec2& circleCenter,
                                              float radius) const = 0;

  [[nodiscard]] virtual std::vector<Line> GenerateLines(
      const glm::vec3& at) const = 0;

  virtual void GetAABB(glm::vec3& min, glm::vec3& max) const = 0;

 protected:
  explicit Volume() = default;
};
}  // namespace soil::world::volume
#endif
