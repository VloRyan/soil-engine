#include "tab_pane.h"
#include "label.h"
#include "stage/scene/gui/container/v_box.h"
namespace soil_samples::common::node {

class VBoxDummy : public soil::stage::scene::gui::container::VBox {
 public:
  VBoxDummy(int margin, glm::ivec4 padding) : VBox(margin, padding) {}

 protected:
  void UpdateSize(const glm::ivec2& maxSize) override {
    Rectangle::UpdateSize(maxSize);
  }

 public:
  void Layout() override {
    VBox::Layout();
  }
};

TabPane::TabPane() : Pane("shapeTileDefault", {
    .BackgroundColor={1.F, 1.F, 1.F, 0.F},
    .BackgroundColorMouseOver = {1.F, 1.F, 1.F, 0.F},
}), selectedTabIndex_(-1), container_(nullptr) {
  SetSizeType(soil::stage::scene::gui::Rectangle::SizeTypes::GrowWithContent);
  Pane::SetMinSize(glm::ivec2(100));
  //container_ = AddChild(new soil::stage::scene::gui::container::VBox(5, glm::ivec4(0)));
  container_ = AddChild(new VBoxDummy(5, glm::ivec4(0)));
  tabContainer_ = container_->AddChild(new soil::stage::scene::gui::container::HBox(5, glm::ivec4(0)));
  tabContainer_->SetRelativeSize(glm::vec2(1.F, 0.F));
  tabContainer_->SetItemAlignment({soil::stage::scene::gui::layout::Alignment::Horizontal::Center,
                                   soil::stage::scene::gui::layout::Alignment::Vertical::Center});
}

void TabPane::AddTab(const std::string& name, Rectangle* content) {
  for (auto& t : tabs_) {
    if (t.Name == name) {
      throw std::runtime_error("tab with name '" + name + "' already exists");
    }
  }
  auto firstTag = tabs_.empty();
  tabs_.push_back({.Name=name, .Content =content});
  auto* tabLabel = tabContainer_->AddChild(new Label(name));
  tabLabel->SetOnClick([this, name](auto button) {
    selectTab(name);
  });
  container_->AddChild(content);
  content->SetVisible(firstTag);
  if (firstTag) {
    selectedTabIndex_ = 0;
  }
}

void TabPane::AddChildNode(soil::stage::scene::Node* node) {
  if (container_ != nullptr) {
    return;// only allow AddTab
  }
  Pane::AddChildNode(node);
}

void TabPane::selectTab(const std::string& name) {
  if (selectedTabIndex_ != -1 && tabs_[selectedTabIndex_].Name == name) {
    return;
  }
  auto prevTabIndex = selectedTabIndex_;
  if (prevTabIndex != -1 && tabs_[prevTabIndex].Content != nullptr) {
    tabs_[prevTabIndex].Content->SetVisible(false);
  }
  selectedTabIndex_ = -1;
  for (auto i = 0; i < tabs_.size(); i++) {
    if (tabs_[i].Name == name) {
      selectedTabIndex_ = i;
    }
  }
  if (selectedTabIndex_ != -1 && tabs_[selectedTabIndex_].Content != nullptr) {
    tabs_[selectedTabIndex_].Content->SetVisible(true);
  }
  auto* parent = GetParentRect();
  while (parent != nullptr && parent->GetParentRect() != nullptr) {
    parent = parent->GetParentRect();
  }
  if (parent != nullptr) {
    parent->SetDirty(DirtyImpact::Dependents);
  } else {
    SetDirty(DirtyImpact::Dependents);
  }
  //container_->SetDirty(soil::stage::scene::Node::DirtyImpact::Dependents);
}
}
