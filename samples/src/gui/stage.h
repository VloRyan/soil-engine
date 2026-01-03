#ifndef GUI_STAGE_H
#define GUI_STAGE_H
#include "common/stage.h"
#include "file/font.h"
#include "file/sprite_sheet.h"
#include "menu/item.h"
#include "stage/scene/component/text/abstract_text.h"
#include "stage/scene/gui/root.h"
#include "stage/stage.h"

namespace soil_samples::gui {
class Stage final : public common::Stage {
 public:
  struct MenuItemDefinition {
    std::string Caption{};
    std::string Value{};
    std::string BackgroundTileName{};
    std::string IconName{};
    std::string ToolTip{};
    float LetterSize{1.F};
    Plane::Style BackgroundStyle{HoverStyle};
    const std::function<void(menu::Item& item)> OnClick;
  };

  static inline auto HoverStyle = Plane::Style{
      .BackgroundColor = glm::vec4(0.8F, 0.8F, 0.8F, 1.F),
      .BackgroundColorMouseOver = glm::vec4(1.F),
  };

  explicit Stage();
  ~Stage() override = default;
  void Render(soil::video::render::State& state) override;

  void GenerateMenu(const std::vector<MenuItemDefinition>& items) const;

  void OnLoad(soil::stage::scene::Scene* scene) override;

 protected:
  void RegisterInputEvents(soil::input::EventMap& eventMap) override;

 private:
  void initGui();
  std::unordered_map<std::string, soil::stage::scene::component::text::Symbol>
  makeSymbolMap(const soil::file::SpriteSheet& spriteSheet,
                const soil::file::Font& font,
                const soil::video::texture::Texture* symbolTexture);

  menu::Item* createMenuItem(const MenuItemDefinition& def) const;
  std::vector<soil::video::texture::Texture*> textures_;
  soil::stage::scene::gui::Root* root_;
  soil::file::SpriteSheet spriteSheet_;
  menu::Menu* mainMenu_;
};
}  // namespace soil_samples::gui

#endif
