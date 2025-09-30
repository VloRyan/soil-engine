#include "plane.h"

namespace soil_samples::gui {
Plane::Plane(const std::string& tilePrefab) : Plane(tilePrefab, Style()) {}

Plane::Plane(const std::string& tilePrefab, const Style& style)
    : background_(nullptr), style_(style), scissorFrom_(nullptr) {
  background_ = AddComponent(new component::ShapeTile(tilePrefab, true));
  background_->SetColor(style_.BackgroundColor);
}

component::ShapeTile& Plane::Background() const { return *background_; }

const soil::video::render::Rect& Plane::GetScissorRect() const {
  if (scissorFrom_ != nullptr) {
    return scissorFrom_->GetScissorRect();
  }
  return Rectangle::GetScissorRect();
}

soil::stage::scene::gui::Rectangle* Plane::GetScissorFrom() const {
  return scissorFrom_;
}

void Plane::SetScissorFrom(Rectangle* const scissorRect) {
  this->scissorFrom_ = scissorRect;
}

void Plane::OnMouseOver(const glm::ivec2& pos) {
  Rectangle::OnMouseOver(pos);
  background_->SetColor(style_.BackgroundColorMouseOver);
}

void Plane::OnMouseOut() {
  Rectangle::OnMouseOut();
  background_->SetColor(style_.BackgroundColor);
}

void Plane::BeforeNodeUpdate() {
  background_->SetVisible(IsVisible());
  Rectangle::BeforeNodeUpdate();
}

void Plane::SetSize(const glm::ivec2& size) {
  Rectangle::SetSize(size);
  background_->SetSize(GetSize());
}

void Plane::SetVisible(const bool visible) {
  Rectangle::SetVisible(visible);
  background_->SetVisible(IsVisible());
}

Plane::Style Plane::GetStyle() const { return style_; }

void Plane::SetStyle(const Style& style) {
  style_ = style;
  background_->SetColor(style_.BackgroundColor);
}

}  // namespace soil_samples::gui
