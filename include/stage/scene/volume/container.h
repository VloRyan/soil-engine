#ifndef SOIL_STAGE_SCENE_VOLUME_CONTAINER_H
#define SOIL_STAGE_SCENE_VOLUME_CONTAINER_H
#include <functional>

#include "stage/scene/hook/update_hook.h"
#include "world/volume/container.h"

namespace soil::stage::scene::component {
class BoundingVolume;
}

namespace soil::stage::scene::volume {
class Container : public hook::UpdateHook {
 public:
  explicit Container(world::volume::Container* container);

  ~Container() override;

  void OnUpdate() override;

  virtual void QueryVolumesAt(
      const glm::vec3& point,
      std::vector<const component::BoundingVolume*>& volumes) const;

  virtual void QueryVolumesInRange(
      const glm::vec3& point, float radius,
      std::vector<const component::BoundingVolume*>& volumes) const;

  void ForEachAt(
      glm::vec3 point,
      const std::function<bool(const component::BoundingVolume* volume)>& f)
      const;

  void ForEachInRange(
      glm::vec3 point, float radius,
      const std::function<bool(const component::BoundingVolume* volume)>& f)
      const;

  [[nodiscard]] const world::volume::Container* GetContainer() const;

  void Handle(const event::Component& event) override;

 protected:
  void OnComponentAdded(component::Component* component) const;

  void OnComponentRemoved(component::Component* component);

 private:
  world::volume::Container* container_;
};
}  // namespace soil::stage::scene::volume

#endif
