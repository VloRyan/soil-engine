#ifndef SOIL_EXAMPLES_GUI_STAGE_H
#define SOIL_EXAMPLES_GUI_STAGE_H
#include "common/node/pane.h"
#include "common/stage.h"
#include "file/font.h"
#include "file/sprite_sheet.h"
#include "menu/menu.h"
#include "stage/scene/component/text/abstract_text.h"
#include "stage/scene/gui/container/flow_box.h"
#include "stage/scene/gui/root.h"
#include "stage/stage.h"
#include "stage/scene/component/sound.h"

namespace soil_samples::gui {
class Stage final : public common::Stage {
 public:
  explicit Stage();
  ~Stage() override = default;
  void Render(soil::video::render::State& state) override;

  void GenerateMenu(
      const std::vector<menu::Menu::MenuItemDefinition>& items) const;

  void OnLoad(soil::stage::scene::Scene* scene) override;
  void Handle(const soil::video::event::WindowEvent& event) override;

 protected:
  void RegisterInputEvents(soil::input::EventMap& eventMap) override;

 private:
  void initGui();

  std::vector<soil::video::texture::Texture*> textures_;
  soil::stage::scene::gui::Root* root_;
  soil::file::SpriteSheet spriteSheet_;
  menu::Menu* mainMenu_;
  menu::Menu* settingsMenu_;
  common::node::Pane* bgIcons_;
  common::node::Pane* iconPlane_;
  inline static auto REPEAT_BG_TILES = 6.F;

  soil::stage::scene::component::Sound* music_;
};
}  // namespace soil_samples::gui

#endif
