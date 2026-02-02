#include "stage.h"
#include <asset.h>
#include <string>

#include "common/component/text_component.h"
#include "glm/glm.hpp"
#include "stage/scene/scene.h"
#include "stage/stage.h"
#include "stage/scene/gui/root.h"
#include "common/component/shape.h"
namespace soil_samples::text {
Stage::Stage()
    : text_(nullptr),
      bgPane_(nullptr),
      description_(),
      bouncingText_(nullptr),
      bounceTextVelocity_(0.F),
      bounceTextGlowVelocity_(0.01F),
      statisticsAsText_(false), fastChangeIndex_(-1) {}

void Stage::OnLoad(soil::stage::scene::Scene* scene) {
  auto* quadVao = GetResources().GetVao("quad");
  auto* bgTexture = GetResources().Textures().GetTexture2D(
      asset::GetPath("Textures/crt.jpg"));
  auto& renderState = GetResources().GetRenderState();
  renderState.SetTexture(0, *bgTexture);

  auto* fontFile = soil::file::Font::Load(asset::GetPath("Fonts/Calibri.fnt"));
  auto* fontTexture =
      GetResources().Textures().GetTexture2D(fontFile->TextureFileName);
  renderState.SetTexture(1, *fontTexture);

  auto* charShader = GetResources().GetShader(
      common::component::TextComponent::CHARACTER_SHADER_NAME);
  renderState.SetShader(charShader);

  spriteSheet_ =
      soil::file::SpriteSheet::Load(asset::GetPath("Textures/Tiles/Gui.json"));
  auto* guiTexture = GetResources().Textures().GetTextureArray2D(
      spriteSheet_.GetTextureFile(), spriteSheet_.FramesPerDim);

  auto* symbolShader = GetResources().GetShader(
      common::component::TextComponent::SYMBOL_SHADER_NAME);
  auto* shader = GetResources().GetShader(common::component::Shape::SHADER_NAME);
  renderState.SetShader(shader);

  common::component::Shape::PREFABS.Insert("textBackground",
                                           {
                                               .QuadVao = quadVao,
                                               .Shader = shader,
                                           });

  soil::stage::scene::component::text::AbstractText::InitPrefab(
      "TextCalibri",
      {
          .QuadVao = quadVao,
          .CharacterShader = charShader,
          .SymbolShader = symbolShader,
          .Font = fontFile,
          .FontTexture = fontTexture,
          .SymbolMap = {{"smiley",
                         {
                             .Name = "smiley",
                             .Texture = guiTexture,
                             .TileIndex = spriteSheet_.FrameByName("smiley"),
                             .AdvanceX = fontFile->Base + fontFile->Padding[0] +
                                 fontFile->Padding[2],
                             .SizeX = fontFile->Base,

                         }},
                        {"party_popper",
                         {
                             .Name = "party_popper",
                             .Texture = guiTexture,
                             .TileIndex =
                             spriteSheet_.FrameByName("party_popper"),
                             .AdvanceX = fontFile->Base + fontFile->Padding[0] +
                                 fontFile->Padding[2],
                             .SizeX = fontFile->Base,
                         }},
                        {"love",
                         {
                             .Name = "love",
                             .Texture = guiTexture,
                             .TileIndex = spriteSheet_.FrameByName("love"),
                             .AdvanceX = fontFile->Base + fontFile->Padding[0] +
                                 fontFile->Padding[2],
                             .SizeX = fontFile->Base,
                         }},
                        {"hot",
                         {
                             .Name = "hot",
                             .Texture = guiTexture,
                             .TileIndex = spriteSheet_.FrameByName("hot"),
                             .AdvanceX = fontFile->Base + fontFile->Padding[0] +
                                 fontFile->Padding[2],
                             .SizeX = fontFile->Base,
                         }}},

      });

  const auto winSize = glm::vec2(GetResources().GetWindow()->GetSize());

  auto* root = scene->AddChild(
      new soil::stage::scene::gui::Root(winSize));

  initBackground(root);

  text_ = bgPane_->AddChild(new common::node::Label("Hello world!", "TextCalibri"));
  text_->Text().SetCharacterSize(1);
  text_->Text().SetMaxLineLength(1080);
  text_->Text().SetColor(glm::vec4(0.F, 0.4F, 0.4F, 0.5F));
  text_->Text().SetBorderColor(glm::vec3(0.0, .6F, .6F));
  text_->Text().SetCharacterOutline(glm::vec2(0.5F, 0.2F));
  text_->Text().SetBorderOutline(glm::vec2(0.5F, 0.2F));

  description_ =
      bgPane_->AddChild(new common::node::Label("Text sample\n"
                                                "Press\n"
                                                "    1 - Show statistics and FPS\n"
                                                "    2 - Toggle long text (lorem ipsum)\n"
                                                "    3 - Text with symbols\n"
                                                "    4 - Text with colors\n"
                                                "    + - Increase text size\n"
                                                "    - - Decrease text size\n", "TextCalibri"));
  description_->Text().SetCharacterSize(0.2);
  description_->Text().SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  description_->SetAnchor({soil::stage::scene::gui::layout::Alignment::Horizontal::Left,
                           soil::stage::scene::gui::layout::Alignment::Vertical::Top});

  bouncingText_ = bgPane_->AddChild(new common::node::Label("Bouncing...", "TextCalibri"));
  bouncingText_->Text().SetCharacterSize(1);
  bouncingText_->Text().SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  bouncingText_->Text().SetCharacterOutline(glm::vec2(0.5F, 0.2F));
  bouncingText_->Text().SetBorderOutline(glm::vec2(0.5F, 0.2F));
  bouncingText_->Text().SetBorderColor(glm::vec3(1, 0, 0));

  bounceTextVelocity_ = glm::vec2(4.F, -4.F);
}

void Stage::RegisterInputEvents(soil::input::EventMap& eventMap) {
  eventMap
      .AddKeyMapping(soil::input::Keys::Key_Plus,
                     soil::input::Event::StateType::Press,
                     [this](const soil::input::Event&) {
                       text_->Text().SetCharacterSize(
                           text_->Text().GetCharacterSize() + 0.1F);
                     })
      .AddKeyMapping(soil::input::Keys::Key_Minus,
                     soil::input::Event::StateType::Press,
                     [this](const soil::input::Event&) {
                       if (text_->Text().GetCharacterSize() > 0.1F) {
                         text_->Text().SetCharacterSize(
                             text_->Text().GetCharacterSize() - 0.1F);
                       }
                     })
      .AddKeyMapping(soil::input::Keys::S,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       printStatistics_ = !printStatistics_;
                     })
      .AddKeyMapping(soil::input::Keys::Key_1,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       statisticsAsText_ = !statisticsAsText_;
                       printStatistics_ = !printStatistics_;
                       if (!printStatistics_) {
                         text_->SetText("Hallo world!");
                       }
                       text_->Text().SetColor(glm::vec4(0.F, 0.4F, 0.4F, 0.5F));
                     })
      .AddKeyMapping(
          soil::input::Keys::Key_2, soil::input::Event::StateType::Release,
          [this](const soil::input::Event&) {
            if (text_->Text().GetText().starts_with("Lorem")) {
              text_->SetText("Hallo world!");
            } else {
              text_->SetText(
                  "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, "
                  "sed diam nonumy eirmod tempor "
                  "invidunt ut labore et dolore magna aliquyam erat, sed diam "
                  "voluptua. At vero eos et "
                  "accusam "
                  "et justo duo dolores et ea rebum. Stet clita kasd "
                  "gubergren, no sea takimata sanctus est "
                  "Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, "
                  "consetetur sadipscing elitr, sed "
                  "diam "
                  "nonumy eirmod tempor invidunt ut labore et dolore magna "
                  "aliquyam erat, sed diam voluptua. "
                  "At "
                  "vero eos et accusam et justo duo dolores et ea rebum. Stet "
                  "clita kasd gubergren, no sea "
                  "takimata sanctus est Lorem ipsum dolor sit amet.");
            }
            text_->Text().SetColor(glm::vec4(0.F, 0.4F, 0.4F, 0.5F));
          })
      .AddKeyMapping(soil::input::Keys::Key_3,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       text_->SetText(
                           "Text with :smiley:_symbols:party_popper:"
                           "\n"
                           "\n:love: this :hot: feature");
                       text_->Text().SetColor(glm::vec4(1.F, 1.F, 1.F, 0.5F));
                     })
      .AddKeyMapping(
          soil::input::Keys::Key_4, soil::input::Event::StateType::Release,
          [this](const soil::input::Event&) {
            text_->SetText(
                "{color=#03fcd3}C{color=#03dbfc}o{color=#8cfc03}l{"
                "color=#fc5a03}o{color=#fc03f8}r{color=#3503fc}f{"
                "color=#03fc24}u{color=#fc0303}l{color} text"
                "\n"
                "\n{color=#ffffffff}f{color=#ffffffd6}a{color=#"
                "ffffff6e}d{color=#ffffff5e}e "
                "{color=#ffffff2e}o{color=#ffffff1e}u{color=#ffffff0e}t");
            text_->Text().SetColor(glm::vec4(1.F, 1.F, 1.F, 0.5F));
          })
      .AddKeyMapping(soil::input::Keys::F,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       if (fastChangeIndex_ == -1) {
                         fastChangeIndex_ = 0;
                       } else {
                         fastChangeIndex_ = -1;
                       }
                     });
}

void Stage::initBackground(soil::stage::scene::Node* parent) {
  bgPane_ = parent->AddChild(new common::node::Pane("textBackground"));
  bgPane_->SetStyle({
                        .BackgroundColor={.2F, .2F, .2F, 1.F},
                        .BackgroundColorMouseOver={.2F, .2F, .2F, 1.F}
                    });

  bgPane_->SetRelativeSize(glm::vec2(1.F));
}

void Stage::OnStatsChanges(const soil::Engine::Statistics& stats) {
  if (!printStatistics_) {
    return;
  }
  if (!statisticsAsText_) {
    common::Stage::OnStatsChanges(stats);
    return;
  }
  text_->SetText(
      "Hello world!\n"
      "FPS:" +
          std::to_string(stats.FPS) +
          "\n"
          " Draws: " +
          std::to_string(stats.DrawCount / stats.FPS) +
          "\n"
          " Vertices: " +
          std::to_string(stats.VertexCount / stats.FPS) +
          "\n"
          " State changes: " +
          std::to_string(stats.StateChanges / stats.FPS) +
          "\n"
          " Update times: " +
          std::to_string(stats.updateInputTime / stats.FPS) + ", " +
          std::to_string(stats.updateStageTime / stats.FPS) + ", " +
          std::to_string(stats.updateVideoTime / stats.FPS) +
          "\n"
          " Render times: " +
          std::to_string(stats.startRenderTime / stats.FPS) + ", " +
          std::to_string(stats.renderTime / stats.FPS) + ", " +
          std::to_string(stats.endRenderTime / stats.FPS));
}

void Stage::Update() {
  const auto winSize =
      glm::vec2(GetResources().GetWindow()->GetSize() - glm::ivec2(30, 30));
  const auto pos = bouncingText_->GetPosition();
  auto newPos = glm::vec2(pos) + bounceTextVelocity_;
  auto halfSize = bouncingText_->Text().GetSize() * glm::vec2(0.5F);
  for (auto i = 0; i < 2; i++) {
    if (newPos[i] + halfSize[i] > winSize[i]) {
      newPos[i] = winSize[i] - halfSize[i];
      bounceTextVelocity_[i] *= -1;
    }
    if (newPos[i] - halfSize[i] < 0) {
      newPos[i] = 0 + halfSize[i];
      bounceTextVelocity_[i] *= -1;
    }
  }
  bouncingText_->SetPosition(glm::vec3(newPos, pos.z));
  auto outline = bouncingText_->Text().GetBorderOutline();
  outline.x += bounceTextGlowVelocity_;
  if (outline.x > 0.8F) {
    bounceTextGlowVelocity_ = -0.01F;
    outline.x = 0.8F;
  }
  if (outline.x < 0.6F) {
    bounceTextGlowVelocity_ = 0.01F;
    outline.x = 0.6F;
  }
  bouncingText_->Text().SetBorderOutline(outline);
  const std::vector<std::string> parts = {"lorem", "ipsum", "dolor", "sit",
                                          "amet"};
  if (fastChangeIndex_ != -1) {
    text_->SetText(parts[fastChangeIndex_++]);
    if (fastChangeIndex_ == parts.size()) {
      fastChangeIndex_ = 0;
    }
  }
  soil::stage::Stage::Update();
}

}  // namespace soil_samples::text
