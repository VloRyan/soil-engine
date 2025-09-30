#ifndef SOIL_WORLD_VOLUME_FRUSTUM_H
#define SOIL_WORLD_VOLUME_FRUSTUM_H

#include <array>
#include <glm/glm.hpp>
#include <vector>

namespace soil::world::volume {
enum class Planes : std::uint8_t {
  Left = 0,
  Right,
  Top,
  Bottom,
  Near,
  Far,
  COUNT
};

enum class Points : std::uint8_t {
  NearTopLeft = 0,
  NearBottomLeft,
  NearBottomRight,
  NearTopRight,
  FarTopLeft,
  FarBottomLeft,
  FarBottomRight,
  FarTopRight,
  COUNT
};

class Frustum final {
 public:
  explicit Frustum(const glm::mat4& projectionView);

  ~Frustum() = default;

  void SetProjectionView(const glm::mat4& pv);

  [[nodiscard]] bool IsVisible(const std::vector<glm::vec3>& point,
                               int count) const;

  [[nodiscard]] bool IsVisible(const glm::vec3& point) const;

  [[nodiscard]] bool IsVisible(const glm::vec3& point, float ratio) const;

  [[nodiscard]] bool IsVisible(Planes plane, const glm::vec3& min,
                               const glm::vec3& max) const;

  [[nodiscard]] bool IntersectBox(const glm::vec3& min,
                                  const glm::vec3& max) const;

  [[nodiscard]] glm::vec4 GetPlane(Planes plane) const;

  [[nodiscard]] glm::vec3 GetPoint(Points point) const;

 private:
  static glm::vec4 normalize(glm::vec4 plane);

  [[nodiscard]] static glm::vec3 intersectionPoint(glm::vec4 planeA,
                                                   glm::vec4 planeB,
                                                   glm::vec4 planeC);

  // Planes are in format: normal(xyz), offset(w)
  std::array<glm::vec4, static_cast<int>(Planes::COUNT)> planes_;
};
}  // namespace soil::world::volume

#endif
