#ifndef TEST_WORLD_VOLUME_MOCKS_HPP
#define TEST_WORLD_VOLUME_MOCKS_HPP
#include "world/volume/container.h"
namespace soil::world::volume {

class ContainerMock : public Container {
 public:
  ContainerMock() = default;

  void Insert(const entity::CollisionObject* object) override {
    Calls.Insert++;
  }

  bool Remove(const entity::CollisionObject* object) override {
    Calls.Remove++;
    return true;
  }

  void QueryObjectsAt(
      const glm::vec3& point,
      std::vector<const entity::CollisionObject*>& objects) const override {
    Calls.QueryObjectsAt++;
  }

  void QueryNodeIndicesFor(const entity::CollisionObject*,
                           std::vector<int>& indices) const override {
    Calls.QueryNodeIndicesFor++;
  }

  void QueryObjectsInRange(
      const glm::vec3& point, float radius,
      std::vector<const entity::CollisionObject*>& vector) const override {
    Calls.QueryObjectsInRange++;
  }

  void GetNodeObjects(
      int index,
      std::vector<const entity::CollisionObject*>& objects) const override {
    Calls.GetNodeObjects++;
  }

  void WalkObjects(
      std::function<void(const entity::CollisionObject*)> fun) const override {
    Calls.WalkObjects++;
  }

  [[nodiscard]] const Node* GetNode(int index) const override {
    Calls.GetNode++;
    return nullptr;
  }

  [[nodiscard]] size_t GetNodeCount() const override {
    Calls.GetNodeCount++;
    return 0;
  }

  [[nodiscard]] size_t GetChildrenPerNode() const override {
    Calls.GetChildrenPerNode++;
    return 0;
  }
  [[nodiscard]] glm::vec3 GetSize() const override { return glm::vec3(0.F); }

  void Clear() override { Calls.Clear++; }

  struct Calls_t {
    int Insert;
    int Remove;
    int QueryObjectsAt;
    int QueryNodeIndicesFor;
    int QueryObjectsInRange;
    int GetNodeObjects;
    int WalkObjects;
    int GetNode;
    int GetNodeCount;
    int GetChildrenPerNode;
    int Clear;
  };

 private:
  Calls_t calls_{};

 public:
  Calls_t& Calls = calls_;
};
}  // namespace soil::world::volume
#endif
