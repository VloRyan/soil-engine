#include "stage.h"

#include <asset.h>
#include <plog/Log.h>

#include <string>

#include "common/component/text_component.h"
#include "common/node/label.h"
#include "file/font.h"
#include "menu/item.h"
#include "menu/menu.h"
#include "stage/scene/component/render/update_matrices_ubo_component.h"
#include "stage/scene/gui/root.h"
#include "stage/scene/scene.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"
#include "common/node/checkbox.h"
#include "stage/scene/gui/container/h_box.h"
#include "common/node/tab_pane.h"
#include "stage/scene/gui/container/v_box.h"
#include "common/component/shape_tile.h"

namespace soil_samples::gui {
constexpr auto UBO_TARGET_MATRICES = 0;

Stage::Stage()
    : root_(nullptr),
      mainMenu_(nullptr),
      bgIcons_(nullptr),
      iconPlane_(nullptr),
      settingsMenu_(nullptr), music_(nullptr) {}

void Stage::OnLoad(soil::stage::scene::Scene* scene) {
  auto* fontFile = soil::file::Font::Load(asset::GetPath("Fonts/Calibri.fnt"));
  auto* fontTexture =
      GetResources().Textures().GetTexture2D(fontFile->TextureFileName);

  spriteSheet_ =
      soil::file::SpriteSheet::Load(asset::GetPath("Textures/Tiles/Gui.json"));
  auto* guiTexture = GetResources().Textures().GetTextureArray2D(
      spriteSheet_.GetTextureFile(), spriteSheet_.FramesPerDim);
  textures_ = {fontTexture, guiTexture};

  auto& state = GetResources().GetRenderState();
  for (auto* texture : textures_) {
    state.SetTexture(*texture);
  }
  const auto viewer = scene->AddChild(new soil::stage::scene::viewer::Ortho(
      GetResources().GetWindow()->GetSize()));
  viewer->Look(glm::vec3(0.F), glm::vec3(0.F, 0.F, -1.F));

  scene->AddComponent(
      new soil::stage::scene::component::render::UpdateMatricesUboComponent(
          viewer, UBO_TARGET_MATRICES, &state));

  auto* quadVao = GetResources().GetVao("quad");

  auto* shapeTileShader =
      GetResources().GetShader(common::component::ShapeTile::SHADER_NAME);

  root_ = scene->AddChild(
      new soil::stage::scene::gui::Root(GetResources().GetWindow()->GetSize()));

  common::component::ShapeTile::PREFABS.Insert("shapeTileDefault",
                                               {
                                                   .QuadVao = quadVao,
                                                   .Shader = shapeTileShader,
                                                   .Texture = guiTexture,
                                               });

  soil::stage::scene::component::text::AbstractText::InitPrefab(
      "Calibri",
      {
          .QuadVao = quadVao,
          .CharacterShader = GetResources().GetShader(
              common::component::TextComponent::CHARACTER_SHADER_NAME),
          .SymbolShader = GetResources().GetShader(
              common::component::TextComponent::SYMBOL_SHADER_NAME),
          .Font = fontFile,
          .FontTexture = fontTexture,
          .SymbolMap =
          soil::stage::scene::component::text::AbstractText::MakeSymbolMap(
              spriteSheet_, *fontFile, guiTexture),
      });

  {
    initGui();
  }

  auto* source = GetResources().GetSource(
      asset::GetPath("Sounds/Path to Lake Land.ogg"), true);
  music_ = scene->AddComponent(new soil::stage::scene::component::Sound(source));
}

void Stage::RegisterInputEvents(soil::input::EventMap& eventMap) {
  eventMap
      .AddKeyMapping(soil::input::Keys::Escape,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       GetResources().GetWindow()->Close();
                     })
      .AddKeyMapping(soil::input::Keys::S,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       printStatistics_ = !printStatistics_;
                     });
}

void Stage::GenerateMenu(
    const std::vector<menu::Menu::MenuItemDefinition>& items) const {
  for (const auto& menuItem : items) {
    mainMenu_->AddMenuItem(menuItem);
  }
  mainMenu_->AddMenuItem({.Id = "settings",
                             .Caption = "Settings",
                             .SpriteSheet = &spriteSheet_,
                             .BackgroundTileName = "button",
                             .IconName = "settings",
                             .LetterSize = 0.6f,
                             .OnClick = [this](auto button) {
                               settingsMenu_->SetVisible(true);
                               mainMenu_->SetVisible(false);
                             }});
  auto pane = new common::node::Pane();
  pane->SetSizeType(soil::stage::scene::gui::Rectangle::SizeTypes::GrowWithContent);
  pane->SetStyle({.BackgroundColor = glm::ivec4(0, 0, 0, 1)});
  auto box = pane->AddChild(new soil::stage::scene::gui::container::VBox());
  box->SetRelativeSize(glm::vec2(1.F, 0.F));
  box->SetItemAlignment({.X=soil::stage::scene::gui::layout::Alignment::Horizontal::Left});
  box->AddChild(new common::node::Label("test"));
  mainMenu_->AddItem(pane);
  /*mainMenu_->AddMenuItem({.Id = "exit",
                             .Caption = "Exit",
                             .SpriteSheet = &spriteSheet_,
                             .BackgroundTileName = "button",
                             .IconName = "exit",
                             .ToolTip = "Exit the program",
                             .LetterSize = 0.6f,
                             .OnClick = [this](auto button) {
                               GetResources().GetWindow()->Close();
                             }});*/
}

void Stage::Render(soil::video::render::State& state) {
  soil::stage::Stage::Render(state);
}

void Stage::Handle(const soil::video::event::WindowEvent& event) {
  soil::stage::Stage::Handle(event);
  if (event.Cause == soil::video::event::WindowEvent::SizeChanged) {
    auto& winSize = event.Window->GetSize();
    auto aspect = static_cast<float>(winSize.x) / static_cast<float>(winSize.y);
    if (aspect < 1.F) {
      bgIcons_->Background().SetFeature(common::component::ShapeComponent::Features::TileScale,
                                        glm::vec2(REPEAT_BG_TILES, REPEAT_BG_TILES / aspect));
    } else {
      bgIcons_->Background().SetFeature(common::component::ShapeComponent::Features::TileScale,
                                        glm::vec2(REPEAT_BG_TILES * aspect, REPEAT_BG_TILES));
    }
  }
}

void Stage::initGui() {
  auto* backgroundPlane = root_->AddChild(new common::node::Pane());
  backgroundPlane->SetRelativeSize(glm::vec2(1.F));
  bgIcons_ = backgroundPlane->AddChild(new common::node::Pane());
  bgIcons_->SetRelativeSize(glm::vec2(0.95F));
  bgIcons_->Background().SetFeature(common::component::ShapeComponent::Features::TileIndex,
                                    spriteSheet_.FrameByName("soil_logo"));
  auto& winSize = GetResources().GetWindow()->GetSize();
  auto aspect = static_cast<float>(winSize.x) / static_cast<float>(winSize.y);
  if (aspect < 1.F) {
    bgIcons_->Background().SetFeature(common::component::ShapeComponent::Features::TileScale,
                                      glm::vec2(REPEAT_BG_TILES, REPEAT_BG_TILES / aspect));
  } else {
    bgIcons_->Background().SetFeature(common::component::ShapeComponent::Features::TileScale,
                                      glm::vec2(REPEAT_BG_TILES * aspect, REPEAT_BG_TILES));
  }

  mainMenu_ = bgIcons_->AddChild(new menu::Menu({
                                                    .Padding = glm::ivec4(40),
                                                    .Scrollbar = {},
                                                    .SpriteSheet = &spriteSheet_,
                                                    .BackgroundTileName = "bg_dark",
                                                    .Style =
                                                        {
                                                            .BackgroundColor = glm::vec4(0.8F, 0.8F, 0.8F, 0.95F),
                                                            .BackgroundColorMouseOver = glm::vec4(1.F),
                                                        },
                                                }));
  mainMenu_->SetAnchor({soil::stage::scene::gui::layout::Alignment::Horizontal::Center,
                        soil::stage::scene::gui::layout::Alignment::Vertical::Center});
  mainMenu_->SetMaxSize(glm::ivec2(800, 768));
  mainMenu_->SetSizeType(
      soil::stage::scene::gui::Rectangle::SizeTypes::GrowWithContent);

  /*iconPlane_ = bgIcons_->AddChild(new common::node::Plane());
  iconPlane_->SetVisible(false);
  iconPlane_->SetAnchor({ soil::stage::scene::gui::layout::Anchor::
                             HorizontalAlignments::Center,
                          soil::stage::scene::gui::layout::Anchor::
                             VerticalAlignments::Bottom});
  iconPlane_->SetMaxSize(
      glm::ivec2(5 * 64 + 4 * 5, std::numeric_limits<int>::max()));
  iconPlane_->SetSizeType(
      soil::stage::scene::gui::Rectangle::SizeTypes::GrowWithContent);
  iconPlane_->SetStyle({.BackgroundColor{0.F, 0.F, 0.F, .5F},
                        .BackgroundColorMouseOver{0.F, 0.F, 0.F, 1.F}});

  auto flowBox =
      iconPlane_->AddChild(new soil::stage::scene::gui::container::FlowBox(5));

  for (auto& frame : spriteSheet_.Frames) {
    auto* icon = flowBox->AddChild(new common::node::Plane());
    icon->SetSize(glm::ivec2(64));
    icon->Background().SetTileIndex(frame.second);
    auto* toolTip = new common::node::Label(frame.first);
    toolTip->SetStyle(common::node::Plane::ToolTipStyle);
    toolTip->Text().SetCharacterSize(0.4F);
    icon->SetToolTip(toolTip);
  }*/

  settingsMenu_ = bgIcons_->AddChild(new menu::Menu({
                                                        .Padding = glm::ivec4(40),
                                                        .SpriteSheet = &spriteSheet_,
                                                        .BackgroundTileName = "bg_dark",
                                                    }));
  settingsMenu_->SetVisible(false);
  settingsMenu_->SetAnchor({soil::stage::scene::gui::layout::
                            Alignment::Horizontal::Center,
                            soil::stage::scene::gui::layout::
                            Alignment::Vertical::Center});
  settingsMenu_->SetMaxSize(glm::ivec2(800, 768));
  settingsMenu_->SetSizeType(
      soil::stage::scene::gui::Rectangle::SizeTypes::GrowWithContent);

  settingsMenu_->AddItem(new common::node::Label("Settings"));

  auto* tabs = settingsMenu_->AddChild(new common::node::TabPane());
  auto* videoSettings = new soil::stage::scene::gui::container::VBox(5);
  videoSettings->SetMinSize(glm::ivec2(0, 120));
  videoSettings->SetItemAlignment({soil::stage::scene::gui::layout::Alignment::Horizontal::Left,
                                   soil::stage::scene::gui::layout::Alignment::Vertical::Top});
  //control->SetOnChanged([this]() { music_->Source().IsPlaying() ? music_->Stop() : music_->Play(); });

  auto* option =
      videoSettings->AddChild(new soil::stage::scene::gui::container::HBox());
  option->SetItemAlignment({soil::stage::scene::gui::layout::Alignment::Horizontal::Left,
                            soil::stage::scene::gui::layout::Alignment::Vertical::Top});
  option->SetMinSize(glm::ivec2(200, 40));
  option->SetRelativeSize(glm::vec2(1.0F, 0.F));
  auto* label = option->AddChild(new common::node::Label("Resolution"));
  label->SetRelativeSize(glm::vec2(0.5F, 0.F));
  label->SetStyle(common::node::Pane::ToolTipStyle);
  label->Text().SetCharacterSize(0.6F);
  auto* checkBoxControl = option->AddChild(new common::node::Checkbox(false,
                                                                      {.SpriteSheet=&spriteSheet_, .BackgroundTile="selection", .CheckMarkTile="check_mark",}));
  checkBoxControl->SetRelativeSize(glm::vec2(0.F, 0.5F));
  checkBoxControl->SetAspectRatio(1.F);
  checkBoxControl->SetMinSize(glm::ivec2(32));
  checkBoxControl->SetMaxSize(glm::ivec2(48));

  option =
      videoSettings->AddChild(new soil::stage::scene::gui::container::HBox());
  option->SetItemAlignment({soil::stage::scene::gui::layout::Alignment::Horizontal::Left,
                            soil::stage::scene::gui::layout::Alignment::Vertical::Top});
  option->SetMinSize(glm::ivec2(200, 40));
  option->SetRelativeSize(glm::vec2(1.0F, 0.F));
  label = option->AddChild(new common::node::Label("Fullscreen"));
  label->SetRelativeSize(glm::vec2(0.5F, 0.F));
  label->SetStyle(common::node::Pane::ToolTipStyle);
  label->Text().SetCharacterSize(0.6F);
  auto* dropDownControl = option->AddChild(new common::node::Label("1920x1080"));
  dropDownControl->Text().SetCharacterSize(0.6);
  //control->SetOnChanged([this]() { music_->Source().IsPlaying() ? music_->Stop() : music_->Play(); });
  tabs->AddTab(":gui:", videoSettings);

  auto* soundSettings = new soil::stage::scene::gui::container::VBox(5);
  soundSettings->SetItemAlignment({soil::stage::scene::gui::layout::Alignment::Horizontal::Left,
                                   soil::stage::scene::gui::layout::Alignment::Vertical::Top});
  soundSettings->SetMinSize(glm::ivec2(0, 120));
  option =
      soundSettings->AddChild(new soil::stage::scene::gui::container::HBox());
  option->SetItemAlignment({soil::stage::scene::gui::layout::Alignment::Horizontal::Left,
                            soil::stage::scene::gui::layout::Alignment::Vertical::Top});
  option->SetMinSize(glm::ivec2(200, 40));
  option->SetRelativeSize(glm::vec2(1.0F, 0.F));
  label = option->AddChild(new common::node::Label("Music"));
  label->SetRelativeSize(glm::vec2(0.5F, 0.F));
  label->SetStyle(common::node::Pane::ToolTipStyle);
  label->Text().SetCharacterSize(0.6F);
  checkBoxControl = option->AddChild(new common::node::Checkbox(false,
                                                                {.SpriteSheet=&spriteSheet_, .BackgroundTile="selection", .CheckMarkTile="check_mark",}));
  checkBoxControl->SetRelativeSize(glm::vec2(0.F, 0.5F));
  checkBoxControl->SetAspectRatio(1.F);
  checkBoxControl->SetMinSize(glm::ivec2(32));
  checkBoxControl->SetMaxSize(glm::ivec2(48));
  checkBoxControl->SetOnChanged([this]() { music_->Source().IsPlaying() ? music_->Stop() : music_->Play(); });
  tabs->AddTab(":sound:", soundSettings);

  auto* footer =
      settingsMenu_->AddItem(new soil::stage::scene::gui::container::HBox());
  footer->SetRelativeSize(glm::vec2(1.F, 0.F));
  footer->SetMinSize(glm::ivec2(200, 40));

  auto* applyButton = footer->AddChild(menu::Menu::CreateMenuItem({
                                                                      .Id = "apply",
                                                                      .Caption = "Apply",
                                                                      .SpriteSheet = &spriteSheet_,
                                                                      .BackgroundTileName = "guiBg",
                                                                      .LetterSize = 0.5F,
                                                                  }));
  applyButton->SetRelativeSize(glm::vec2(0.5F, 1.F));
  auto* backButton = footer->AddChild(menu::Menu::CreateMenuItem({
                                                                     .Id = "back",
                                                                     .Caption = "Back",
                                                                     .SpriteSheet = &spriteSheet_,
                                                                     .BackgroundTileName = "guiBg",
                                                                     .ToolTip = "Back to main menu",
                                                                     .LetterSize = 0.5F,
                                                                     .OnClick =
                                                                     [this](soil::input::MouseButton button) {
                                                                       settingsMenu_->SetVisible(false);
                                                                       mainMenu_->SetVisible(true);
                                                                     },
                                                                 }));
  backButton->SetRelativeSize(glm::vec2(0.5F, 1.F));
}

}  // namespace soil_samples::gui
