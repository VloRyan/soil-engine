#ifndef SOIL_EXAMPLES_GUI_MENU_MENU_H
#define SOIL_EXAMPLES_GUI_MENU_MENU_H

#include "common/node/label.h"
#include "common/node/pane.h"
#include "file/sprite_sheet.h"
#include "scrollbar.h"
#include "stage/scene/gui/container/base.h"
#include "stage/scene/gui/rectangle.h"

namespace soil_samples::gui::menu {
class Item;

class Menu : public common::node::Pane {
 public:
  enum class Orientation : std::uint8_t {
    Vertical = 0,
    Horizontal,
  };

  struct ScrollBarDefinition {
    bool Enabled{true};
    int Width{30};
  };

  struct Definition {
    Menu::Orientation Orientation{Orientation::Vertical};
    int Margin{10};
    glm::ivec4 Padding{0};
    ScrollBarDefinition Scrollbar{.Enabled = false};
    soil::file::SpriteSheet* SpriteSheet{nullptr};
    std::string BackgroundTileName;
    Pane::Style Style{};
  };

  struct MenuItemDefinition {
    std::string Id{};
    std::string Caption{};
    const soil::file::SpriteSheet* SpriteSheet{nullptr};
    std::string BackgroundTileName{};
    std::string IconName{};
    std::string ToolTip{};
    float LetterSize{1.F};
    common::node::Pane::Style BackgroundStyle{common::node::Pane::HoverStyle};
    common::node::Label::TextStyle LabelStyle{common::node::Label::HoverStyle};
    const std::function<void(soil::input::MouseButton button)> OnClick;
    glm::vec4 Padding{0.F};
  };

  explicit Menu(const Definition& definition);
  ~Menu() override = default;

  [[nodiscard]] virtual glm::vec2 GetScrollOffset() const;
  virtual void SetScrollOffset(const glm::vec2& scrollOffset);
  [[nodiscard]] virtual int GetSelectedItemIndex() const;
  virtual void SetSelectedItemIndex(int selectedItemIndex);
  void OnSelectionChanged(
      const std::function<void(int current, int prev)>& onSelectionChanged);
  [[nodiscard]] virtual Item* GetItem(int index) const;

  static menu::Item* CreateMenuItem(const MenuItemDefinition& def);
  Item* AddMenuItem(const MenuItemDefinition& def);
  Rectangle* AddItem(Rectangle* item);

 protected:
  void AddChildNode(Node* node) override;

  void OnMouseOver(const glm::ivec2& pos) override;
  void OnMouseOut() override;
  void OnMouseWheel(const glm::ivec2& pos, glm::vec2 offset) override;
  void OnMouseButton(const glm::ivec2& pos, soil::input::MouseButton button,
                     soil::input::Event::StateType state) override;

  void AfterNodeUpdate() override;
  Definition definition_;
  glm::ivec2 scrollOffset_;
  // Scrollbar* verticalScrollbar_;
  int selectedItemIndex_;
  int maxItems_;
  std::function<void(int current, int prev)> onSelectionChanged_;
  soil::stage::scene::gui::container::Base* container_;
  std::unordered_map<std::string, Rectangle*> items_;
};
}  // namespace soil_samples::gui::menu

#endif
