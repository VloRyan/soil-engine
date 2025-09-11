#include "stage/scene/gui/container/h_box.h"

namespace soil::stage::scene::gui::container {

    HBox::HBox(const int margin, const glm::ivec4 padding) : Base(margin, padding), alignItems_(AlignItems::Center) {}

    HBox::AlignItems HBox::GetAlignItems() const {
        return alignItems_;
    }

    void HBox::SetAlignItems(const AlignItems alignItems) {
        if (alignItems_ == alignItems) {
            return;
        }
        alignItems_ = alignItems;
        SetDirty(DirtyImpact::Dependents);
    }

    void HBox::arrangeItems() {
        itemsSize_ = glm::vec2(0.F);
        if (items_.empty()) {
            return;
        }

        auto offset = glm::ivec2(padding_[0] + GetOffset().x, GetOffset().y);
        const auto halfWidth = GetSize().x / 2;
        for (auto* item : GetItems()) {
            auto pos = glm::ivec2(item->GetPosition());
            pos.x = offset.x - halfWidth + item->GetSize().x / 2;
            pos.y = offset.y;
            switch (alignItems_) {
            case AlignItems::Top:
                pos.y += GetSize().y / 2 - padding_[1] - item->GetSize().y / 2;
                break;
            case AlignItems::Center:
                pos.y += padding_[1] - padding_[3];
                break;
            case AlignItems::Bottom:
                pos.y -= GetSize().y / 2 - padding_[3] - item->GetSize().y / 2;
                break;
            }
            item->SetPosition(glm::vec3(pos, GetPosition().z));
            offset.x += item->GetSize().x + margin_;
            itemsSize_.x += item->GetSize().x;
            if (item->GetSize().y > itemsSize_.y) {
                itemsSize_.y = item->GetSize().y;
            }
        }
        itemsSize_.x += (static_cast<int>(items_.size()) - 1) * margin_;
    }
} // namespace soil::stage::scene::gui::container
