#include "checkbox.h"
#include "stage/scene/gui/layout/sizer.h"
namespace soil_samples::common::node {
Checkbox::Checkbox(bool checked, const Style& style) : checked_(checked), onChanged_(nullptr) {
  Background().SetFeature(common::component::ShapeComponent::Features::TileIndex,
                          style.SpriteSheet->FrameByName(style.BackgroundTile));
  checkMark_ = AddComponent(new component::ShapeTile("shapeTileDefault", true));
  checkMark_->SetTileIndex(style.SpriteSheet->FrameByName(style.CheckMarkTile));
  checkMark_->SetVisible(checked_);

  childrenSize_ = glm::ivec2(-1);
}

bool Checkbox::IsChecked() const { return checked_; }

void Checkbox::SetChecked(bool checked) {
  if (checked_ == checked) {
    return;
  }
  checked_ = checked;
  SetDirty(soil::stage::scene::Node::DirtyImpact::Self);
}

void Checkbox::OnMouseButton(const glm::ivec2& pos,
                             soil::input::MouseButton button,
                             soil::input::Event::StateType state) {
  Pane::OnMouseButton(pos, button, state);
  if (button == soil::input::MouseButton::Left && state == soil::input::Event::StateType::Release) {
    SetChecked(!IsChecked());
    if (onChanged_ != nullptr) {
      onChanged_();
    }
  }
}

void Checkbox::BeforeNodeUpdate() {
  Pane::BeforeNodeUpdate();
  checkMark_->SetVisible(checked_ && IsVisible());
  checkMark_->SetSize(GetSize() - glm::ivec2(10));
}

void Checkbox::Layout() {
  Pane::Layout();
  checkMark_->SetPositionOffset(
      glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_COMPONENT_INCREMENT));
}

glm::ivec2 Checkbox::CalculateSize(const glm::ivec2& maxSize) {
  if (childrenSize_ == glm::ivec2(-1)) {
    UpdateSize(maxSize);
  }
  return GetSize();
}

glm::ivec2 Checkbox::CalculateChildrenSize(const glm::ivec2& maxSize) {
  return childrenSize_;
}

void Checkbox::UpdateSize(const glm::ivec2& maxSize) {
  //Plane::UpdateSize(maxSize);
  switch (sizeType_) {
    case Rectangle::SizeTypes::Fixed:
      childrenSize_ = size_;
      break;
    case Rectangle::SizeTypes::Relative: {
      childrenSize_ = glm::clamp(soil::stage::scene::gui::layout::Sizer::CalculateRelativeSize(
          GetRelativeSize(), GetAspectRatio(), maxSize, maxSize_), minSize_, maxSize_) - Paddings();
      break;
    }
    case Rectangle::SizeTypes::GrowWithContent: {
      childrenSize_ = glm::clamp(maxSize, minSize_, maxSize_) - Paddings();
      break;
    }
  }
  SetSize(childrenSize_ + Paddings());
}

void Checkbox::SetSize(const glm::ivec2& size) {
  Pane::SetSize(size);
}

void Checkbox::SetOnChanged(const std::function<void()>& onChanged) {
  onChanged_ = onChanged;
}

}  // namespace soil_samples::common::node
