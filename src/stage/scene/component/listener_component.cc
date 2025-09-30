
#include "stage/scene/component/listener_component.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
ListenerComponent::ListenerComponent(sound::Listener* listener,
                                     const bool setOrientation)
    : Component(Type::Sound),
      listener_(listener),
      setOrientation_(setOrientation) {}

sound::Listener* ListenerComponent::GetListener() const { return listener_; }

void ListenerComponent::UpdateTransform(const glm::mat4& matrix) {
  Component::UpdateTransform(matrix);
  listener_->SetPosition(matrix[3]);
  if (setOrientation_) {
    listener_->SetOrientation(glm::mat2x3(matrix[2], matrix[1]));
  }
}
}  // namespace soil::stage::scene::component
