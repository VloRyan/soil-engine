#ifndef SOIL_EXAMPLES_COMMON_NODE_TAB_PANE_H
#define SOIL_EXAMPLES_COMMON_NODE_TAB_PANE_H
#include "pane.h"
#include "stage/scene/gui/container/h_box.h"
namespace soil_samples::common::node {
class TabPane : public Pane {
 public:
  TabPane();
  ~TabPane() override = default;
  void AddTab(const std::string& name, Rectangle* content);
 protected:
  void AddChildNode(Node* node) override;
  void selectTab(const std::string& name);
 private:
  struct Tab {
    std::string Name;
    Rectangle* Content{nullptr};
  };
  soil::stage::scene::gui::container::Base* container_;
  std::vector<Tab> tabs_;
  soil::stage::scene::gui::container::HBox* tabContainer_;
  int selectedTabIndex_;
};
}

#endif
