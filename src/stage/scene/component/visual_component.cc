#include "stage/scene/component/visual_component.h"

namespace soil::stage::scene::component {
    VisualComponent::VisualComponent(const render::Type renderType, const bool opaque, const bool visible) :
        Component(Type::Visual), renderType_(renderType), visible_(visible), opaque_(opaque) {}

    render::Type VisualComponent::GetRenderType() const {
        return renderType_;
    }

    bool VisualComponent::IsVisible() const {
        return visible_;
    }

    void VisualComponent::SetVisible(const bool visible) {
        if (visible_ == visible) {
            return;
        }
        visible_ = visible;
        SetDirty();
    }

    bool VisualComponent::IsOpaque() const {
        return opaque_;
    }

    void VisualComponent::SetOpaque(const bool opaque) {
        if (opaque_ == opaque) {
            return;
        }
        opaque_ = opaque;
        SetDirty();
    }
} // namespace soil::stage::scene::component
