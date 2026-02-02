#include "label.h"

#include "common/component/text_component.h"
#include "pane.h"

namespace soil_samples::common::node {
Label::Label(const std::string& text, const std::string& font)
    : Pane("shapeTileDefault", DefaultStyle), text_(nullptr) {
  text_ = AddComponent(new component::TextComponent(font, text));
  Pane::SetSize(text_->GetSize());
  SetSizeType(soil::stage::scene::gui::Rectangle::SizeTypes::GrowWithContent);
  text_->SetPositionOffset(
      glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_COMPONENT_INCREMENT));
}

void Label::SetText(const std::string& text) {
  text_->SetText(text);
  SetDirty(soil::stage::scene::Node::DirtyImpact::Self);
}

component::TextComponent& Label::Text() const { return *text_; }

void Label::BeforeNodeUpdate() {
  text_->SetVisible(IsVisible());
  auto halfSize = glm::vec2(GetSize()) * glm::vec2(0.5F);
  auto textHalfSize = glm::vec2(text_->GetSize()) * glm::vec2(0.5F);
  text_->SetPositionOffset(glm::vec3(-halfSize.x + textHalfSize.x + padding_[0],
                                     0.F,
                                     Rectangle::LAYER_Z_COMPONENT_INCREMENT));
  Pane::BeforeNodeUpdate();
}

void Label::OnMouseOver(const glm::ivec2& pos) {
  Pane::OnMouseOver(pos);
  if (textStyle_.has_value()) {
    text_->SetColor(textStyle_->ColorMouseOver);
  }
}

void Label::OnMouseOut() {
  Pane::OnMouseOut();
  if (textStyle_.has_value()) {
    text_->SetColor(textStyle_->Color);
  }
}

const Label::TextStyle* Label::GetTextStyle() const {
  if (textStyle_.has_value()) {
    return &textStyle_.value();
  }
  return nullptr;
}

void Label::SetTextStyle(const Label::TextStyle& textStyle) {
  textStyle_ = textStyle;
  if (textStyle_.has_value() && IsMouseOver()) {
    text_->SetColor(textStyle_->ColorMouseOver);
  } else {
    text_->SetColor(textStyle_->Color);
  }
}

glm::ivec2 Label::CalculateChildrenSize(const glm::ivec2& maxSize) {
  return text_->GetSize();
}

}  // namespace soil_samples::common::node
