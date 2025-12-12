#ifndef SOIL_STAGE_SCENE_COMPONENT_WORLD_WORLD_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_WORLD_WORLD_COMPONENT_H

#include "collision_object_component.h"
#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"
#include "stage/scene/component/event/hook.h"
#include "stage/scene/node.h"
#include "world/world.h"
namespace soil::stage::scene {
class Scene;
}
namespace soil::stage::scene::component::world {

class WorldComponent : public component::Component {
 public:
  explicit WorldComponent(soil::world::World* world);
  ~WorldComponent() override;
  void Handle(const stage::event::Component& event);
  void OnEvent(const stage::event::Node& event);
  void OnTrigger(const stage::hook::TriggerHook::TriggerPoint& point);
  void Activate(CollisionObjectComponent* comp);
  void UpdatePosition(CollisionObjectComponent* comp);

  void ForEachAt(
      glm::vec3 point,
      const std::function<bool(const CollisionObjectComponent* volume)>& f)
      const;

  void ForEachInRange(
      glm::vec3 point, float radius,
      const std::function<bool(const CollisionObjectComponent* volume)>& f)
      const;
  CollisionObjectComponent* ResolveCollisionObjectComponent(
      const soil::world::entity::CollisionObject* object) const;
  void Clear();
  const soil::world::volume::Container* Container() const;

 protected:
  void Insert(CollisionObjectComponent* comp);
  void Remove(CollisionObjectComponent* comp);
  void RemoveAllDependentWorldComponents(Node* node);

  void OnStageChanged(class Stage* stage, class Stage* prevStage) override;
  bool IsBelowMe(Node* node) const;
  void SetParent(Node* parent) override;

 private:
  event::Hook hook_;
  soil::world::World* world_;
  // TODO
  std::unordered_map<const soil::world::entity::CollisionObject*,
                     CollisionObjectComponent*>
      objectToComponentMap_;
  bool linked_;
};

}  // namespace soil::stage::scene::component::world

#endif
