#ifndef SOIL_EXAMPLES_GUI_MENU_ITEM_H
#define SOIL_EXAMPLES_GUI_MENU_ITEM_H
#include "common/node/pane.h"

namespace soil_samples::gui::menu {
class Menu;

class Item : public common::node::Pane {
 public:
  explicit Item(const std::string& id = "");
  ~Item() override = default;
  [[nodiscard]] virtual std::string Id() const;

 private:
  std::string id_;
};

}  // namespace soil_samples::gui::menu

#endif
