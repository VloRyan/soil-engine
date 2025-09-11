#include "menu.h"

#include "item.h"
#include "scrollbar.h"
#include "stage/scene/gui/container/h_box.h"
#include "stage/scene/gui/container/v_box.h"

namespace soil_samples::gui::menu {
    Menu::Menu(const Definition& definition) :
        definition_(definition), bgPlane_(nullptr), scrollOffset_(0), verticalScrollbar_(nullptr),
        selectedItemIndex_(-1), maxItems_(3), onSelectionChanged_(nullptr) {
        Rectangle::SetPadding(definition.Padding);
        if (definition.Orientation == Orientation::Vertical) {
            container_ = AddChild(new soil::stage::scene::gui::container::VBox(definition.Margin, glm::ivec4(0)));
        } else {
            container_ = AddChild(new soil::stage::scene::gui::container::HBox(definition.Margin, glm::ivec4(0)));
        }
        container_->SetRelativeSize(glm::vec2(1.F));
        container_->SetPosition(glm::vec3(Menu::GetCenter(), LAYER_Z_INCREMENT));

        bgPlane_ = AddChild(new Plane());
        bgPlane_->SetPosition(glm::vec3(0.F)); // no z-offset
        bgPlane_->Background().SetTileIndex(definition_.SpriteSheet->TileIndex(definition.BackgroundTileName));
        bgPlane_->SetStyle(definition.Style);
        bgPlane_->SetScissorFrom(this);
        if (definition.Scrollbar.Enabled) {
            verticalScrollbar_ = AddChild(new Scrollbar({
                .SpriteSheet = definition.SpriteSheet,
                .BackgroundTileName = "test",
                .Scroller =
                    {
                        .TileName = "sphere",
                        .Style =
                            {
                                .BackgroundColor = glm::vec4(0.0F, 0.2F, 0.9F, 1.F),
                                .BackgroundColorMouseOver = glm::vec4(0.0F, 0.3F, 1.F, 1.F),
                            },
                    },
            }));
            verticalScrollbar_->SetVisible(false);
            verticalScrollbar_->SetRelativeSize(glm::vec2(0.F, 1.F));
            verticalScrollbar_->SetSize(glm::vec2(definition.Scrollbar.Width, 0));
            verticalScrollbar_->SetOnValueChange(
                [this](const float value) { container_->SetOffset(glm::vec2(0, value)); });
            verticalScrollbar_->SetScissorFrom(this);
        }
    }

    void Menu::OnMouseOver(const glm::ivec2& pos) {
        if (!IsVisible()) {
            return;
        }
        Rectangle::OnMouseOver(pos);
    }

    void Menu::OnMouseOut() {
        if (!IsVisible()) {
            return;
        }
        Rectangle::OnMouseOut();
    }

    void Menu::OnMouseWheel(const glm::ivec2& pos, const glm::vec2 offset) {
        if (!IsVisible()) {
            return;
        }
        Rectangle::OnMouseWheel(pos, offset);
        if (verticalScrollbar_ != nullptr && verticalScrollbar_->IsVisible()) {
            auto direction = Scrollbar::Direction::Down;
            if (offset.y > 0.F) {
                direction = Scrollbar::Direction::Up;
            }
            verticalScrollbar_->Scroll(direction);
        }
    }

    void Menu::OnMouseButton(const glm::ivec2& pos, const soil::input::MouseButton button,
                             const soil::input::Event::State state) {
        if (!IsVisible()) {
            return;
        }
        Rectangle::OnMouseButton(pos, button, state);
        if (state == soil::input::Event::State::Release) {
            for (int i = 0; i < container_->GetItems().size(); i++) {
                if (const auto* item = container_->GetItems()[i]; item->Contains(pos)) {
                    if (i != selectedItemIndex_) {
                        SetSelectedItemIndex(i);
                    } else {
                        SetSelectedItemIndex(-1);
                    }
                    break;
                }
            }
        }
    }

    void Menu::BeforeNodeUpdate() {
        if (verticalScrollbar_ != nullptr) {
            if (container_->GetChildSize().y < container_->GetItemsSize().y) {
                verticalScrollbar_->SetVisible(true);
                verticalScrollbar_->SetMinValue(0.F);
                verticalScrollbar_->SetMaxValue(
                    static_cast<float>(container_->GetItemsSize().y - container_->GetChildSize().y));
                verticalScrollbar_->SetIncrement(10.F);
                auto pos = verticalScrollbar_->GetPosition();
                pos.x = static_cast<float>(GetSize().x) * 0.5F - static_cast<float>(definition_.Scrollbar.Width) * 0.5F;
                verticalScrollbar_->SetPosition(pos);
            } else {
                verticalScrollbar_->SetVisible(false);
            }
        }
        container_->SetPosition(glm::vec3(Menu::GetCenter(), LAYER_Z_INCREMENT));
        Rectangle::BeforeNodeUpdate();
    }

    glm::vec2 Menu::GetScrollOffset() const {
        return scrollOffset_;
    }

    void Menu::SetScrollOffset(const glm::vec2& scrollOffset) {
        scrollOffset_ = scrollOffset;
    }

    int Menu::GetSelectedItemIndex() const {
        return selectedItemIndex_;
    }

    void Menu::SetSelectedItemIndex(const int selectedItemIndex) {
        if (selectedItemIndex_ == selectedItemIndex) {
            return;
        }
        const auto prev = selectedItemIndex_;
        selectedItemIndex_ = selectedItemIndex;
        if (onSelectionChanged_) {
            onSelectionChanged_(selectedItemIndex_, prev);
        }
    }

    void Menu::OnSelectionChanged(const std::function<void(int current, int prev)>& onSelectionChanged) {
        onSelectionChanged_ = onSelectionChanged;
    }

    Item* Menu::GetItem(const int index) const {
        return dynamic_cast<Item*>(container_->GetItems().at(index));
    }

    void Menu::SetSize(const glm::ivec2& size) {
        Rectangle::SetSize(size);
        bgPlane_->SetSize(size);
    }

    glm::ivec2 Menu::GetChildSize() const {
        auto childSize = Rectangle::GetChildSize();
        if (verticalScrollbar_ != nullptr && verticalScrollbar_->IsVisible()) {
            childSize -= glm::ivec2(definition_.Scrollbar.Width, 0.F);
        }
        return childSize;
    }

    glm::vec2 Menu::GetCenter() const {
        auto center = Rectangle::GetCenter();
        if (verticalScrollbar_ != nullptr && verticalScrollbar_->IsVisible()) {
            center.x -= definition_.Scrollbar.Width * 0.5F;
        }
        return center;
    }

    void Menu::addChild(Node* node) {
        if (node->GetParent() == this) {
            return;
        }
        if (dynamic_cast<Item*>(node) != nullptr) {
            container_->AddChild(node);
            SetDirty(DirtyImpact::Dependents);
        } else {
            Rectangle::addChild(node);
        }
    }

    void Menu::UpdateScissor(const soil::video::render::Rect& parentRect) {
        Rectangle::UpdateScissor(parentRect);
    }

} // namespace soil_samples::gui::menu
