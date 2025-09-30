#ifndef SOIL_WORLD_VOLUME_AABB_H
#define SOIL_WORLD_VOLUME_AABB_H

#include <array>

#include "glm/glm.hpp"
#include "volume.hpp"

namespace soil::world::volume {
class AABB final : public Volume {
 public:
  enum class Point : std::uint8_t {
    TOP_FRONT_LEFT = 0,
    TOP_BACK_LEFT,
    TOP_BACK_RIGHT,
    TOP_FRONT_RIGHT,
    BOTTOM_FRONT_LEFT,
    BOTTOM_BACK_LEFT,
    BOTTOM_BACK_RIGHT,
    BOTTOM_FRONT_RIGHT
  };

  explicit AABB(glm::vec3 dimension);

  ~AABB() override;

  AABB(const AABB& other) = delete;

  AABB(AABB&& other) noexcept = delete;

  AABB& operator=(const AABB& other) = delete;

  AABB& operator=(AABB&& other) noexcept = delete;

  void GetTopPlane(std::array<glm::vec4, 4> points) const;

  void GetBottomPlane(std::array<glm::vec4, 4> points) const;

  void GetAllPoints(std::array<glm::vec4, 8> points) const;

  void SetDimension(glm::vec3 dimension);

  void SetPosition(const glm::vec3& position) override;

  [[nodiscard]] glm::vec3 GetPosition() const override;

  [[nodiscard]] glm::vec3 GetPoint(Point point) const;

  [[nodiscard]] bool IsInside(const glm::vec3& min,
                              const glm::vec3& max) const override;

  [[nodiscard]] bool IsInsideXZ(const glm::vec3& min,
                                const glm::vec3& max) const override;

  [[nodiscard]] bool IsInside(const glm::vec2& min,
                              const glm::vec2& max) const override;

  [[nodiscard]] bool Contains(const glm::vec3& point) const override;

  [[nodiscard]] bool ContainsXZ(const glm::vec3& point) const override;

  [[nodiscard]] IntersectionResult IntersectsRay(
      const glm::vec3& start, const glm::vec3& dir) const override;

  [[nodiscard]] IntersectionResult IntersectsRayXZ(
      const glm::vec3& start, const glm::vec3& dir) const override;

  [[nodiscard]] bool IntersectsCircle(const glm::vec2& circleCenter,
                                      float radius) const override;

  [[nodiscard]] std::vector<Line> GenerateLines() const override;

 private:
  void UpdateWorldPoints(const glm::vec3& position);

  std::array<glm::vec3, 8> points_;
  std::array<glm::vec4, 8> worldPoints_;
  glm::vec3 minPoint_;

 public:
  [[nodiscard]] const glm::vec3& GetMinPoint() const;

  [[nodiscard]] const glm::vec3& GetMaxPoint() const;

 private:
  glm::vec3 maxPoint_;
};
}  // namespace soil::world::volume

#endif
