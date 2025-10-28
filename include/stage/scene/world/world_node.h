#ifndef SOIL_STAGE_SCENE_WORLD_NODE_H
#define SOIL_STAGE_SCENE_WORLD_NODE_H

#include "stage/scene/component/collision_object_component.h"
#include "stage/scene/hook/hook.h"
#include "stage/scene/node.h"
#include "world/world.h"
namespace soil::stage::scene {
class Scene;
}
namespace soil::stage::scene::world {

class WorldNode : public scene::Node, public scene::hook::Hook {
 public:
  explicit WorldNode(soil::world::World* world);
  ~WorldNode() override;
  void Handle(const stage::event::Component& event) override;
  void Perform(hook::Hook::Trigger_t trigger) override;
  void Activate(component::CollisionObjectComponent* comp);
  void UpdatePosition(component::CollisionObjectComponent* comp);

  void ForEachAt(
      glm::vec3 point,
      const std::function<
          bool(const component::CollisionObjectComponent* volume)>& f) const;

  void ForEachInRange(
      glm::vec3 point, float radius,
      const std::function<
          bool(const component::CollisionObjectComponent* volume)>& f) const;
  component::CollisionObjectComponent* ResolveCollisionObjectComponent(
      const soil::world::entity::CollisionObject* object) const;
  void Clear();
  const soil::world::volume::Container* Container() const;

 protected:
  void LinkToScene(Scene& scene);
  void Insert(component::CollisionObjectComponent* comp);
  void Remove(component::CollisionObjectComponent* comp);

  void SetParent(Node* parent) override;

  Scene* ClimbUpToScene();
  bool IsBelowMe(Node* node) const;

 private:
  soil::world::World* world_;
  // TODO
  std::unordered_map<const soil::world::entity::CollisionObject*,
                     component::CollisionObjectComponent*>
      objectToComponentMap_;
  bool linked_;
};

}  // namespace soil::stage::scene::world

#endif
