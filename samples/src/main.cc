#include "asset.h"
#include "basic/stage.h"
#include "common/globals.hpp"
#include "gui/stage.h"
#include "instancing/stage.h"
#include "line/stage.h"
#include "sound/stage.h"
#include "text/stage.h"
#include "world/stage.h"
#include "common/component/shape_tile.h"

int parseIntOrDefault(const std::string& s, const int defaultValue) {
  try {
    const int i{std::stoi(s)};
    return i;
  } catch (const std::exception&) {
    return defaultValue;
  }
}

struct StageLoader {
  std::string Id;
  std::string Name;
  std::string IconName;
  std::string ToolTip;
  std::function<soil_samples::common::Stage*()> NewStage;
};

int main(const int argc, const char* argv[]) {
  auto engine = soil::Engine({.Context = {
      .Size = glm::ivec2(1920, 1080),
      .OpenGLVersion = glm::ivec2(3, 3),
  }});
  auto* vidMgr = engine.GetVideoManager();

  vidMgr->PrepareShader(new soil::video::shader::Program(
      soil_samples::common::component::Shape::SHADER_NAME, asset::GetPath("Shader/")));
  vidMgr->PrepareShader(new soil::video::shader::Program(
      soil_samples::instancing::ShapeInstance::SHADER_NAME,
      asset::GetPath("Shader/")));
  vidMgr->PrepareShader(new soil::video::shader::Program(
      soil_samples::common::component::ShapeTile::SHADER_NAME,
      asset::GetPath("Shader/")));
  vidMgr->PrepareShader(new soil::video::shader::Program(
      soil_samples::common::component::TextComponent::CHARACTER_SHADER_NAME,
      asset::GetPath("Shader/")));
  vidMgr->PrepareShader(new soil::video::shader::Program(
      soil_samples::common::component::TextComponent::SYMBOL_SHADER_NAME,
      asset::GetPath("Shader/")));
  vidMgr->PrepareShader(new soil::video::shader::Program(
      soil_samples::line::LineInstance::SHADER_NAME,
      asset::GetPath("Shader/")));

  constexpr uint bufferSize = 4 * sizeof(glm::mat4);
  vidMgr->NewUniformBufferObject("Matrices", bufferSize, UBO_TARGET_MATRICES);

  const std::vector<StageLoader> stagesWithDesc{
      {
          .Id = "default",
          .Name = "Gui",
          .IconName = "gui",
          .NewStage = [] { return new soil_samples::gui::Stage(); },
      },
      {
          .Id = "basic",
          .Name = "Basic",
          .IconName = "basic",
          .NewStage = [] { return new soil_samples::basic::Stage(); },
      },
      {
          .Id = "instancing",
          .Name = "Instancing",
          .IconName = "instancing",
          .NewStage = [] { return new soil_samples::instancing::Stage(); },
      },
      {
          .Id = "text",
          .Name = "Text",
          .IconName = "text",
          .NewStage = [] { return new soil_samples::text::Stage(); },
      },
      {
          .Id = "line",
          .Name = "Line",
          .IconName = "line",
          .NewStage = [] { return new soil_samples::line::Stage(); },
      },
      {
          .Id = "sound",
          .Name = "Sound",
          .IconName = "sound",
          .NewStage = [] { return new soil_samples::sound::Stage(); },
      },
      {
          .Id = "world",
          .Name = "World",
          .IconName = "world",
          .ToolTip = "Physics simulation :rocket:",
          .NewStage = [] { return new soil_samples::world::Stage(); },
      },
  };

  int stageIndex = -1;
  if (argc == 2) {
    stageIndex = parseIntOrDefault(argv[1], -1);
  }
  if (stageIndex == -1) {
    std::vector<soil_samples::gui::menu::Menu::MenuItemDefinition> menuItems;
    soil_samples::gui::Stage* defaultStage = nullptr;
    std::vector<soil::stage::Stage*> stages;
    for (const auto& option : stagesWithDesc) {
      if (option.Id == "default") {
        auto* stage = option.NewStage();
        defaultStage = dynamic_cast<soil_samples::gui::Stage*>(stage);
        engine.GetStageManager()->RegisterStage(option.Id, defaultStage);
        break;
      }
    }
    if (defaultStage == nullptr) {
      throw std::runtime_error("failed to get default stage");
    }
    auto backToDefault = [&engine] {
      engine.GetStageManager()->SetCurrent("default");
    };
    auto spriteSheet = soil::file::SpriteSheet::Load(
        asset::GetPath("Textures/Tiles/Gui.json"));
    for (const auto& option : stagesWithDesc) {
      if (option.Id == "default") {
        continue;
      }
      auto* stage = option.NewStage();
      engine.GetStageManager()->RegisterStage(option.Id, stage);
      stage->SetBackAction(backToDefault);
      menuItems.push_back({
                              .Id = option.Id,
                              .Caption = option.Name,
                              .SpriteSheet = &spriteSheet,
                              .BackgroundTileName = "button",
                              .IconName = option.IconName,
                              .ToolTip = option.ToolTip,
                              .LetterSize = 0.6f,
                              .OnClick = [stage](auto button) { stage->SetCurrent(); },
                          });
    }

    defaultStage->Load();
    defaultStage->GenerateMenu(menuItems);
    engine.GetStageManager()->SetCurrent("default");
  } else {
    auto* stage = stagesWithDesc[stageIndex].NewStage();
    engine.GetStageManager()->RegisterStage(stagesWithDesc[stageIndex].Id,
                                            stage);
    engine.GetStageManager()->SetCurrent(stagesWithDesc[stageIndex].Id);
  }
  engine.Run();
  return 0;

}  // namespace soil_samples
