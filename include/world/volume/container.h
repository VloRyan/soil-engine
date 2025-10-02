#ifndef SOIL_WORLD_VOLUME_CONTAINER_H
#define SOIL_WORLD_VOLUME_CONTAINER_H
#include <vector>

#include "volume.hpp"
#include "world/intersection.h"

namespace soil::world::volume {
class Container {
 public:
  inline static auto NO_CHILDREN = -1;

  struct Node {
    [[nodiscard]] bool Contains(const glm::vec2& point) const {
      return Intersection::Test(
          Intersection::Box2d{
              .Min = Min,
              .Max = Max,
          },
          point);
    }

    [[nodiscard]] bool Contains(const glm::vec2& min,
                                const glm::vec2& max) const {
      return Intersection::Test(
          Intersection::Box2d{
              .Min = Min,
              .Max = Max,
          },
          Intersection::Box2d{
              .Min = min,
              .Max = max,
          });
    }

    [[nodiscard]] bool IntersectsCircle(const glm::vec2& circleCenter,
                                        const float radius) const {
      return Intersection::Test(
          Intersection::Box2d{
              .Min = Min,
              .Max = Max,
          },
          Intersection::Circle2d{
              .Center = circleCenter,
              .Radius = radius,
          });
    }

    inline static uint UNSET = -1;
    uint ChildrenStartIndex{UNSET};
    uint VolumesIndex{UNSET};
    glm::vec2 Min{0.F};
    glm::vec2 Max{0.F};
  };

  virtual ~Container() = default;

  virtual void Insert(const Volume* volume) = 0;

  virtual bool Remove(const Volume* volume) = 0;

  virtual void QueryVolumesAt(const glm::vec3& point,
                              std::vector<const Volume*>& volumes) const = 0;

  virtual void QueryNodeIndicesFor(const Volume*,
                                   std::vector<int>& indices) const = 0;

  virtual void QueryVolumesInRange(
      const glm::vec3& point, float radius,
      std::vector<const Volume*>& vector) const = 0;

  virtual void GetNodeVolumes(int index,
                              std::vector<const Volume*>& volumes) const = 0;

  virtual void WalkVolumes(std::function<void(const Volume*)> fun) const = 0;

  [[nodiscard]] virtual const Node* GetNode(int index) const = 0;

  [[nodiscard]] virtual size_t GetNodeCount() const = 0;

  [[nodiscard]] virtual size_t GetChildrenPerNode() const = 0;

  void Clear();

 protected:
  Container() = default;
};
}  // namespace soil::world::volume
#endif
