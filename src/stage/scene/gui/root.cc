#include "stage/scene/gui/root.h"

#include "window.h"

namespace soil::stage::scene::gui {
    Root::Root(const glm::ivec2 windowSize) {
        size_ = windowSize;
        scissorRect_.Size = size_;
        childScissorRect_.Size = size_;
        Node::SetReceiverType(ReceiverType::Input, true);
        Node::SetReceiverType(ReceiverType::Window, true);
        const auto parentHalfSize = size_ / glm::ivec2(2);
        Node::SetPosition(glm::vec3(parentHalfSize, -TOP_Z_LAYER));
        Object3d::ComputeWorldTransform(glm::mat4(1.F));
    }

    void Root::Handle(const input::Event& event) {
        if (event.GetOrigin() == input::Event::Origin::MouseButton ||
            event.GetOrigin() == input::Event::Origin::MousePosition ||
            event.GetOrigin() == input::Event::Origin::MouseWheel) {
            const auto pos = glm::vec2(event.GetCursorPos().x,
                                       static_cast<float>(size_.y) - event.GetCursorPos().y); // origin to bottom-left
            if (!Contains(pos)) {
                if (isMouseOver_) {
                    OnMouseOut();
                }
                return;
            }
            switch (event.GetOrigin()) {
            case input::Event::Origin::MouseButton:
                OnMouseButton(pos, event.GetMouseButton(), event.GetState());
                break;
            case input::Event::Origin::MousePosition:
                OnMouseOver(pos);
                break;
            case input::Event::Origin::MouseWheel:
                OnMouseWheel(pos, event.GetWheelOffset());
            default:;
            }
        }
        Node::Handle(event);
    }

    void Root::Handle(const WindowEvent& event) {
        if (event.GetCause() != WindowEvent::SizeChanged) {
            return;
        }
        size_ = event.GetWindow()->GetSize();
        scissorRect_.Size = size_;
        childScissorRect_.Size = size_;
        const auto parentHalfSize = size_ / glm::ivec2(2);
        SetPosition(glm::vec3(parentHalfSize, -TOP_Z_LAYER));
        Node::Handle(event);
    }


} // namespace soil::stage::scene::gui
