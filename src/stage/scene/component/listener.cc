
#include "stage/scene/component/listener.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
    Listener::Listener(sound::Listener *listener) : Component(Type::Sound), listener_(listener) {}

    sound::Listener *Listener::GetListener() const { return listener_; }

    void Listener::UpdateTransform(const glm::mat4 &matrix) {
        Component::UpdateTransform(matrix);
        listener_->SetPosition(matrix[3]);
        listener_->SetOrientation(glm::mat2x3(matrix[2], matrix[1]));
    }
} // namespace soil::stage::scene::component
