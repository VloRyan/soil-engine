#include "scrollbar.h"

namespace soil_samples::gui::menu {
    Scrollbar::Scrollbar(const Definition& definition) :
        scroller_(nullptr), value_(0.F), minValue_(0.F), maxValue_(0.F), increment_(0.F), onValueChange_(nullptr) {
        Background().SetTileIndex(definition.SpriteSheet->TileIndex(definition.BackgroundTileName));
        scroller_ = AddChild(new Plane());
        scroller_->Background().SetTileIndex(definition.SpriteSheet->TileIndex(definition.Scroller.TileName));
        scroller_->SetRelativeSize(glm::vec2(0.80F, 0.05F));
        scroller_->SetStyle(definition.Scroller.Style);
        scroller_->SetScissorFrom(Plane::GetScissorFrom());
        UpdateScrollerPosition();
    }

    void Scrollbar::OnMouseButton(const glm::ivec2& pos, const soil::input::MouseButton button,
                                  const soil::input::Event::State state) {
        Plane::OnMouseButton(pos, button, state);
        if (!scroller_->Contains(pos)) {
            auto direction = Direction::Down;
            if (pos.y > scroller_->GetWorldPosition().y) {
                direction = Direction::Up;
            }
            Scroll(direction);
        }
    }

    float Scrollbar::GetValue() const {
        return value_;
    }

    void Scrollbar::SetOnValueChange(const std::function<void(float)>& onValueChange) {
        onValueChange_ = onValueChange;
    }

    void Scrollbar::SetValue(const float value) {
        const auto newValue = std::clamp(value, minValue_, maxValue_);
        if (newValue == value_) {
            return;
        }
        value_ = newValue;
        if (onValueChange_) {
            onValueChange_(value_);
        }
        UpdateScrollerPosition();
    }

    void Scrollbar::UpdateSize(const glm::ivec2& parentSize) {
        Plane::UpdateSize(parentSize);
        UpdateScrollerPosition();
    }

    float Scrollbar::GetMinValue() const {
        return minValue_;
    }

    void Scrollbar::SetMinValue(const float minValue) {
        if (minValue > minValue_) {
            throw std::invalid_argument("minValue must be lesser than maxValue");
        }
        minValue_ = minValue;
    }

    float Scrollbar::GetMaxValue() const {
        return maxValue_;
    }

    void Scrollbar::SetMaxValue(const float maxValue) {
        if (maxValue < minValue_) {
            throw std::invalid_argument("maxValue must be greater than minValue");
        }
        maxValue_ = maxValue;
    }

    float Scrollbar::GetIncrement() const {
        return increment_;
    }

    void Scrollbar::SetIncrement(const float increment) {
        increment_ = increment;
    }

    void Scrollbar::Scroll(Direction direction) {
        if (increment_ == 0.F) {
            return;
        }
        auto newValue = value_;
        const auto scrollStep = (maxValue_ - minValue_) / increment_;
        newValue += scrollStep * static_cast<float>(direction);
        newValue = std::clamp(newValue, minValue_, maxValue_);
        if (newValue != value_) {
            value_ = newValue;
            if (onValueChange_) {
                onValueChange_(value_);
            }
            UpdateScrollerPosition();
        }
    }

    void Scrollbar::SetScissorFrom(Rectangle* rectangle) {
        Plane::SetScissorFrom(rectangle);
        scroller_->SetScissorFrom(rectangle);
    }

    void Scrollbar::UpdateScrollerPosition() const {
        auto pos = scroller_->GetPosition();
        const auto halfHeight = GetSize().y * 0.5F;
        const auto scrollerHalfHeight = scroller_->GetSize().y * 0.5F;
        pos.y = halfHeight - scrollerHalfHeight;
        if (value_ > minValue_ && maxValue_ > 0.F) {
            const auto factor = (value_ / maxValue_);
            const auto scrollTotalHeight = GetSize().y - scroller_->GetSize().y;
            const auto height = scrollTotalHeight * factor;
            pos.y -= height;
        }
        scroller_->SetPosition(pos);
    }

} // namespace soil_samples::gui::menu
