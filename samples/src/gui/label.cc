#include "label.h"

#include "component/text.h"
#include "plane.h"

namespace soil_samples::gui {
    auto constexpr DefaultStyle = Plane::Style {
        .BackgroundColor = glm::vec4(0.F),
        .BackgroundColorMouseOver = glm::vec4(0.F),
    };

    Label::Label(const std::string& text, const std::string& font) : Plane("gui", DefaultStyle), text_(nullptr) {
        text_ = AddComponent(new component::Text(font, text));
        text_->SetPositionOffset(glm::vec3(0.0f, 0.0f, LAYER_Z_COMPONENT_INCREMENT));
    }

    void Label::SetVisible(const bool visible) {
        Plane::SetVisible(visible);
        text_->SetVisible(IsVisible());
    }

    void Label::SetText(const std::string& text) const {
        text_->SetText(text);
    }

    component::Text& Label::Text() const {
        return *text_;
    }

    void Label::BeforeNodeUpdate() {
        text_->SetVisible(IsVisible());
        text_->Update();
        SetSize(text_->GetSize());
        Plane::BeforeNodeUpdate();
    }
} // namespace soil_samples::gui
