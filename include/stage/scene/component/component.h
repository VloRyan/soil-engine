#ifndef SOIL_STAGE_SCENE_COMPONENT_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_COMPONENT_H
#include <glm/glm.hpp>

#include "event/observable.hpp"
#include "stage/event/component.h"
namespace soil::stage {
class Stage;
namespace scene {
class Node;
}
}  // namespace soil::stage

namespace soil::stage::scene::component {
class Component : public soil::event::Observable<event::Component> {
  friend class scene::Node;

 public:
  enum class Type : std::uint8_t {
    // TODO rename
    Transform = 0,
    Drawable,
    WorldEntity,
    Interaction,
    Sound,
    Metadata,
    Lighting,
    World,
    Container,
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
  virtual void OnStageChanged(Stage* stage, Stage* prevStage);
  Node* parent_;

 private:
  Type type_;
  UpdateType updateType_;
};
}  // namespace soil::stage::scene::component

#endif
