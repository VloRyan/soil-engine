#include "item.h"

#include <utility>

#include "menu.h"

namespace soil_samples::gui::menu {
    Item::Item(std::string value) : value_(std::move(value)), onClick_(nullptr) {}

    std::string Item::GetValue() const {
        return value_;
    }

    void Item::SetOnClick(const std::function<void(Item& item)>& onClick) {
        onClick_ = onClick;
    }

    void Item::OnMouseButton(const glm::ivec2& pos, const soil::input::MouseButton button,
                             soil::input::Event::State state) {
        if (onClick_ && //
            state == soil::input::Event::State::Release && //
            button == soil::input::MouseButton::Left) {
            onClick_(*this);
        }
        Plane::OnMouseButton(pos, button, state);
    }
} // namespace soil_samples::gui::menu
