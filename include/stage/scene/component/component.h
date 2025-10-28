#ifndef SOIL_STAGE_SCENE_COMPONENT_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_COMPONENT_H
#include <glm/glm.hpp>

#include "event/observable.hpp"
#include "stage/event/component.h"

namespace soil::stage::scene {
class Node;
}

namespace soil::stage::scene::component {
class Component : public soil::event::Observable<event::Component> {
  friend class scene::Node;

 public:
  enum class Type : std::uint8_t {
    Transform = 0,
    Visual,
    WorldEntity,
    Interaction,
    Sound,
    Metadata,
    Lighting,
    World,
    Any = 255
  };

  enum class UpdateType : std::uint8_t {
    WhenNodeDirty = 0,
    Always,
  };

  explicit Component(Type type);

  ~Component() override;

  [[nodiscard]] Node* GetParent() const;

  [[nodiscard]] Type GetType() const;

  virtual void Update();

  [[nodiscard]] virtual UpdateType GetUpdateType() const;
  virtual void SetUpdateType(UpdateType updateType);

 protected:
  virtual void SetParent(Node* parent);
  virtual void SignalChanged();
  Node* parent_;

 private:
  Type type_;
  UpdateType updateType_;
};
}  // namespace soil::stage::scene::component

#endif
