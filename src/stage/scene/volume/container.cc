#include "stage/scene/volume/container.h"

#include <stdexcept>

#include "stage/scene/component/bounding_volume.h"

namespace soil::stage::scene::volume {
Container::Container(world::volume::Container* container)
    : UpdateHook(HandlerType::Component), container_(container) {}

Container::~Container() { delete container_; }

void Container::Handle(const event::Component& event) {
  switch (event.GetChangeType()) {
    case event::Component::ChangeType::Added:
      OnComponentAdded(event.GetOrigin());
      break;
    case event::Component::ChangeType::Removed:
      OnComponentRemoved(event.GetOrigin());
      break;
    default:;  // nothing
  }
}

void Container::OnComponentAdded(component::Component* component) const {
  if (component->GetType() != component::Component::Type::BoundingVolume) {
    return;
  }
  auto* volume = dynamic_cast<component::BoundingVolume*>(component);
  if (volume == nullptr) {
    return;
  }
  if (volume->GetContainer() == nullptr) {
    container_->Insert(volume);
  } else {
    throw std::runtime_error("volume already added to container");
  }
  volume->SetContainer(container_);
}

void Container::OnComponentRemoved(component::Component* component) {
  if (component->GetType() != component::Component::Type::BoundingVolume) {
    return;
  }
  auto* volume = dynamic_cast<component::BoundingVolume*>(component);
  if (volume == nullptr) {
    return;
  }
  volume->SetContainer(nullptr);
}

void Container::OnUpdate() {
#ifdef DEBUG
  if (container_ == nullptr) {
    throw std::runtime_error("container shall not be nullptr");
  }
#endif
}

void Container::QueryVolumesAt(
    const glm::vec3& point,
    std::vector<const component::BoundingVolume*>& volumes) const {
  std::vector<const world::volume::Volume*> tmpVolumes;
  container_->QueryVolumesAt(point, tmpVolumes);
  volumes.reserve(volumes.size() + tmpVolumes.size());
  for (const auto* volume : tmpVolumes) {
    auto* bv = dynamic_cast<const component::BoundingVolume*>(volume);
#ifdef DEBUG
    if (bv == nullptr) {
      throw std::runtime_error("bv shall not be nullptr");
    }
#endif
    volumes.push_back(bv);
  }
}

void Container::QueryVolumesInRange(
    const glm::vec3& point, const float radius,
    std::vector<const component::BoundingVolume*>& volumes) const {
  std::vector<const world::volume::Volume*> tmpVolumes;
  container_->QueryVolumesInRange(point, radius, tmpVolumes);
  volumes.reserve(volumes.size() + tmpVolumes.size());
  for (const auto* volume : tmpVolumes) {
    auto* bv = dynamic_cast<const component::BoundingVolume*>(volume);
#ifdef DEBUG
    if (bv == nullptr) {
      throw std::runtime_error("bv shall not be nullptr");
    }
#endif
    volumes.push_back(bv);
  }
}

void Container::ForEachAt(
    const glm::vec3 point,
    const std::function<bool(const component::BoundingVolume* volume)>& f)
    const {
  std::vector<const world::volume::Volume*> volumes;
  container_->QueryVolumesAt(point, volumes);
  for (const auto* volume : volumes) {
    if (const auto ret =
            f(dynamic_cast<const component::BoundingVolume*>(volume));
        !ret) {
      return;
    }
  }
}

void Container::ForEachInRange(
    const glm::vec3 point, const float radius,
    const std::function<bool(const component::BoundingVolume* volume)>& f)
    const {
  std::vector<const world::volume::Volume*> volumes;
  container_->QueryVolumesInRange(point, radius, volumes);
  for (const auto* volume : volumes) {
    if (const auto ret =
            f(dynamic_cast<const component::BoundingVolume*>(volume));
        !ret) {
      return;
    }
  }
}

const world::volume::Container* Container::GetContainer() const {
  return container_;
}
}  // namespace soil::stage::scene::volume
