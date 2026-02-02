#include "item.h"

#include "menu.h"

namespace soil_samples::gui::menu {
Item::Item(const std::string& id) : id_(id) {}

std::string Item::Id() const { return id_; }

}  // namespace soil_samples::gui::menu
