#include "pane.h"

#include "stage/scene/gui/root.h"
#include "common/component/shape_tile.h"
#include "common/component/shape.h"

namespace soil_samples::common::node {
Pane::Pane(const std::string& backgroundPrefab) : Pane(backgroundPrefab, Style()) {}

Pane::Pane(const std::string& backgroundPrefab, const Style& style)
    : background_(nullptr), style_(style), toolTip_(nullptr) {
  if (component::ShapeTile::PREFABS.Has(backgroundPrefab)) {
    background_ = AddComponent(new component::ShapeTile(backgroundPrefab, true));
  } else if (component::Shape::PREFABS.Has(backgroundPrefab)) {
    background_ = AddComponent(new component::Shape(backgroundPrefab, true));
  } else {
    throw std::runtime_error("unknown prefab: " + backgroundPrefab);
  }
  background_->SetColor(style_.BackgroundColor);
}

Pane::~Pane() {
  auto root = GuiRoot();
  if (toolTip_ != nullptr && root != nullptr) {
    root->RemoveOverlay(toolTip_);
    delete toolTip_;
  }
}

component::ShapeComponent& Pane::Background() const { return *background_; }

void Pane::OnMouseOver(const glm::ivec2& pos) {
  Rectangle::OnMouseOver(pos);
  background_->SetColor(style_.BackgroundColorMouseOver);
  if (toolTip_ != nullptr && toolTip_->GetParentRect() != nullptr) {
    toolTip_->SetVisible(true);
    toolTip_->SetLocalPosition(
        glm::vec3(toolTip_->GetParentRect()->GetSize() / glm::ivec2(-2) + pos +
                      toolTip_->GetSize() / glm::ivec2(2, -1),
                  toolTip_->GetLocalPosition().z));
  }
}

void Pane::OnMouseOut() {
  Rectangle::OnMouseOut();
  background_->SetColor(style_.BackgroundColor);
  if (toolTip_ != nullptr) {
    toolTip_->SetVisible(false);
  }
}

void Pane::OnMouseButton(const glm::ivec2& pos,
                         const soil::input::MouseButton button,
                         const soil::input::Event::StateType state) {
  Rectangle::OnMouseButton(pos, button, state);
  if (onClick_ != nullptr && state == soil::input::Event::StateType::Release) {
    onClick_(button);
  }
}

void Pane::BeforeNodeUpdate() {
  Rectangle::BeforeNodeUpdate();
  background_->SetVisible(IsVisible());
  background_->SetSize(GetSize());
}

Pane::Style Pane::GetStyle() const { return style_; }

void Pane::SetStyle(const Style& style) {
  style_ = style;
  background_->SetColor(style_.BackgroundColor);
}

void Pane::SetOnClick(
    const std::function<void(soil::input::MouseButton button)>& onClick) {
  onClick_ = onClick;
}

Pane* Pane::GetToolTip() const { return toolTip_; }

void Pane::SetToolTip(Pane* toolTip) {
  if (toolTip_ == toolTip) {
    return;
  }
  delete toolTip_;
  toolTip_ = toolTip;
  if (toolTip_ != nullptr) {
    toolTip_->SetVisible(false);
    auto root = GuiRoot();
    if (root != nullptr) {
      root->AddOverlay(toolTip_);
    }
  }
}

void Pane::OnParentChanged(soil::stage::scene::Node* parent, soil::stage::scene::Node* prevParent) {
  if (toolTip_ != nullptr && toolTip_->GetParent() == nullptr) {
    auto root = GuiRoot();
    if (root != nullptr) {
      root->AddOverlay(toolTip_);
    }
  }
}

}  // namespace soil_samples::common::node

