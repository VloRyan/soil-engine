#include "menu.h"

#include "common/node/label.h"
#include "item.h"
#include "stage/scene/gui/container/h_box.h"
#include "stage/scene/gui/container/v_box.h"

namespace soil_samples::gui::menu {
Menu::Menu(const Definition& definition)
    : definition_(definition),
      scrollOffset_(0),
      container_(nullptr),
    /*verticalScrollbar_(nullptr),*/
      selectedItemIndex_(-1),
      maxItems_(3),
      onSelectionChanged_(nullptr) {
  Pane::SetPadding(definition.Padding);
  if (definition.Orientation == Orientation::Vertical) {
    container_ = AddChild(new soil::stage::scene::gui::container::VBox(
        definition.Margin, glm::ivec4(0)));
  } else {
    container_ = AddChild(new soil::stage::scene::gui::container::HBox(
        definition.Margin, glm::ivec4(0)));
  }
  container_->SetAnchor(
      {soil::stage::scene::gui::layout::Alignment::Horizontal::Left,
       soil::stage::scene::gui::layout::Alignment::Vertical::Top});

  Background().SetFeature(common::component::ShapeComponent::Features::TileIndex,
                          definition_.SpriteSheet->FrameByName(definition.BackgroundTileName));
  Pane::SetStyle(definition.Style);
  Background().SetFeature(common::component::ShapeComponent::Features::RoundingRadius, 0.1F);
  /*if (definition.Scrollbar.Enabled) {
    verticalScrollbar_ = AddChild(new Scrollbar({
        .SpriteSheet = definition.SpriteSheet,
        .BackgroundTileName = "test",
        .Scroller =
            {
                .TileName = "sphere",
                .Style =
                    {
                        .BackgroundColor = glm::vec4(0.0F, 0.2F, 0.9F, 1.F),
                        .BackgroundColorMouseOver =
                            glm::vec4(0.0F, 0.3F, 1.F, 1.F),
                    },
            },
    }));
    verticalScrollbar_->SetVisible(false);
    verticalScrollbar_->SetRelativeSize(glm::vec2(0.F, 1.F));
    verticalScrollbar_->SetSize(glm::vec2(definition.Scrollbar.Width, 0));
    verticalScrollbar_->SetOnValueChange([this](const float value) {
      container_->SetOffset(glm::vec2(0, value));
    });
    verticalScrollbar_->SetScissorFrom(this);
}*/
}

void Menu::OnMouseOver(const glm::ivec2& pos) {
  if (!IsVisible()) {
    return;
  }
  Pane::OnMouseOver(pos);
}

void Menu::OnMouseOut() {
  if (!IsVisible()) {
    return;
  }
  Pane::OnMouseOut();
}

void Menu::OnMouseWheel(const glm::ivec2& pos, const glm::vec2 offset) {
  if (!IsVisible()) {
    return;
  }
  Pane::OnMouseWheel(pos, offset);
  /* if (verticalScrollbar_ != nullptr && verticalScrollbar_->IsVisible()) {
     auto direction = Scrollbar::Direction::Down;
     if (offset.y > 0.F) {
       direction = Scrollbar::Direction::Up;
     }
     verticalScrollbar_->Scroll(direction);
   }*/
}

void Menu::OnMouseButton(const glm::ivec2& pos,
                         const soil::input::MouseButton button,
                         const soil::input::Event::StateType state) {
  if (!IsVisible()) {
    return;
  }
  Pane::OnMouseButton(pos, button, state);
  if (state == soil::input::Event::StateType::Release) {
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

void Menu::AfterNodeUpdate() {
  /* if (verticalScrollbar_ != nullptr) {
     if (container_->GetChildSize().y < container_->GetItemsSize().y) {
       verticalScrollbar_->SetVisible(true);
       verticalScrollbar_->SetMinValue(0.F);
       verticalScrollbar_->SetMaxValue(static_cast<float>(
           container_->GetItemsSize().y - container_->GetChildSize().y));
       verticalScrollbar_->SetIncrement(10.F);
       auto pos = verticalScrollbar_->GetPosition();
       pos.x = static_cast<float>(GetSize().x) * 0.5F -
               static_cast<float>(definition_.Scrollbar.Width) * 0.5F;
       verticalScrollbar_->SetPosition(pos);
     } else {
       verticalScrollbar_->SetVisible(false);
     }
   }*/
  Pane::AfterNodeUpdate();
}

glm::vec2 Menu::GetScrollOffset() const { return scrollOffset_; }

void Menu::SetScrollOffset(const glm::vec2& scrollOffset) {
  scrollOffset_ = scrollOffset;
}

int Menu::GetSelectedItemIndex() const { return selectedItemIndex_; }

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

void Menu::OnSelectionChanged(
    const std::function<void(int current, int prev)>& onSelectionChanged) {
  onSelectionChanged_ = onSelectionChanged;
}

Item* Menu::GetItem(const int index) const {
  return dynamic_cast<Item*>(container_->GetItems().at(index));
}

void Menu::AddChildNode(Node* node) {
  if (node->GetParent() == this) {
    return;
  }
  if (container_ != nullptr && dynamic_cast<Rectangle*>(node) != nullptr) {
    container_->AddChild(node);
    SetDirty(DirtyImpact::Dependents);
  } else {
    Pane::AddChildNode(node);
  }
}

menu::Item* Menu::CreateMenuItem(const MenuItemDefinition& def) {
  auto* item = new Item(def.Id);
  item->SetStyle(def.BackgroundStyle);
  item->SetRelativeSize(glm::vec2(1.0F, 0.0F));
  item->SetAspectRatio(5.F / 1.F);
  item->SetOnClick(def.OnClick);
  item->SetMaxSize(glm::ivec2(400, 80));
  if (!def.BackgroundTileName.empty()) {
    if (def.SpriteSheet == nullptr) {
      throw std::runtime_error("no sprite sheet");
    }
    item->Background().SetFeature(common::component::ShapeComponent::Features::TileIndex,
                                  def.SpriteSheet->FrameByName("bg_button"));
    item->Background().SetFeature(common::component::ShapeComponent::Features::RoundingRadius, 0.05F);
    item->SetPadding(glm::ivec4(30, 0, 30, 0));
  }

  auto* label = item->AddChild(new common::node::Label(
      !def.IconName.empty() ? ":" + def.IconName + ":" + def.Caption
                            : def.Caption));
  label->Text().SetCharacterSize(def.LetterSize);
  label->SetAnchor(
      {
          soil::stage::scene::gui::layout::Alignment::Horizontal::Left,

          soil::stage::scene::gui::layout::Alignment::Vertical::Center}

  );
  label->SetTextStyle(def.LabelStyle);
  if (!def.ToolTip.empty()) {
    auto* toolTip = new common::node::Label(def.ToolTip);
    toolTip->SetStyle(Pane::ToolTipStyle);
    toolTip->Text().SetCharacterSize(0.4F);
    item->SetToolTip(toolTip);
  }
  return item;
}

Item* Menu::AddMenuItem(const MenuItemDefinition& def) {
  return container_->AddChild(CreateMenuItem(def));
}

soil::stage::scene::gui::Rectangle* Menu::AddItem(
    soil::stage::scene::gui::Rectangle* item) {
  return container_->AddChild(item);
}
}  // namespace soil_samples::gui::menu
