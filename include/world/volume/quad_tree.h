#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "base.h"
#include "container.h"

namespace soil::world::volume {

class QuadTree : public Container {
 public:
  explicit QuadTree(float size, byte maxLevel = 4, float minChildSize = 6.0F);
  ~QuadTree() override = default;

  void ReserveSize(float size);

  void Insert(const entity::CollisionObject* object) override;

  void Insert2(const entity::CollisionObject* object);

  bool Remove(const entity::CollisionObject* object) override;

  void QueryObjectsAt(
      const glm::vec3& point,
      std::vector<const entity::CollisionObject*>& objects) const override;

  void QueryObjectsAt(
      const glm::vec2& point,
      std::vector<const entity::CollisionObject*>& objects) const;

  void QueryObjectsInRange(
      const glm::vec3& point, float radius,
      std::vector<const entity::CollisionObject*>& objects) const override;

  void QueryObjectsInRange(
      const glm::vec2& point, float radius,
      std::vector<const entity::CollisionObject*>& objects) const;

  void QueryNodeIndicesFor(const entity::CollisionObject*,
                           std::vector<int>& indices) const override;

  const Node* GetNode(int index) const override;

  void GetNodeObjects(
      int index,
      std::vector<const entity::CollisionObject*>& objects) const override;

  void WalkObjects(
      std::function<void(const entity::CollisionObject*)> fun) const override;

  size_t GetVolumeCount() const;

  size_t GetNodeCount() const override;

  size_t GetChildrenPerNode() const override;

  byte DetermineLevel(std::uint16_t index) const;

  glm::vec3 GetSize() const override;

  void Clear() override;

 private:
  void resize(float size);

  struct state {
    uint Index;
    byte Level;
  };
  bool insert(const entity::CollisionObject* object, int index, byte level);

  void split(std::uint16_t index);

  void split2(std::uint16_t index, byte level);

  static bool isInsideCircle(const glm::vec2& point,
                             const glm::vec2& circleCenter, float radius);

  std::uint16_t freeNodeObjectsIndex();

  std::vector<Node> nodes_;
  std::vector<std::vector<const entity::CollisionObject*>> nodeObjects_;
  std::vector<std::uint16_t> freeNodeObjects_;

  byte maxLevel_;
  float minChildSize_;
};
}  // namespace soil::world::volume

#endif
