#include "asset.h"
#include "basic/shader.h"
#include "basic/stage.h"
#include "gui/character_shader.h"
#include "gui/shape_tile_instance_shader.h"
#include "gui/shape_tile_shader.h"
#include "gui/stage.h"
#include "instancing/shader.h"
#include "instancing/stage.h"
#include "line/shader.h"
#include "line/stage.h"
#include "text/stage.h"
#include "volume/stage.h"

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
    std::function<soil_samples::common::Stage*()> NewStage;
};

constexpr auto UBO_TARGET_MATRICES = 0;

int main(const int argc, const char* argv[]) {
    constexpr auto winParams = soil::WindowParameter{.Size = glm::ivec2(1920, 1080),
                                                     .RenderSize = glm::ivec2(1920, 1080),
                                                     .Type = soil::WindowType::Windowed,
                                                     .OpenGLVersion = glm::ivec2(3, 3)};
    const auto engine = soil::Engine(winParams);
    auto* vidMgr = engine.GetVideoManager();
    vidMgr->PrepareShader(new soil_samples::basic::Shader(asset::GetPath("Shader/")));
    vidMgr->PrepareShader(new soil_samples::instancing::Shader(asset::GetPath("Shader/")));
    vidMgr->PrepareShader(new soil_samples::gui::ShapeTileShader(asset::GetPath("Shader/")));
    vidMgr->PrepareShader(new soil_samples::gui::ShapeTileInstanceShader(asset::GetPath("Shader/")));
    vidMgr->PrepareShader(new soil_samples::gui::CharacterShader(asset::GetPath("Shader/")));
    vidMgr->PrepareShader(new soil_samples::line::Shader(asset::GetPath("Shader/")));

    constexpr uint bufferSize = 4 * sizeof(glm::mat4);
    vidMgr->NewUniformBufferObject("Matrices", bufferSize, UBO_TARGET_MATRICES);

    const std::vector<StageLoader> stagesWithDesc{
        {
            .Id = "default",
            .Name = "Gui",
            .NewStage = [] { return new soil_samples::gui::Stage(); },
        },
        {
            .Id = "basic",
            .Name = "Basic",
            .NewStage = [] { return new soil_samples::basic::Stage(); },
        },
        {
            .Id = "instancing",
            .Name = "Instancing",
            .NewStage = [] { return new soil_samples::instancing::Stage(); },
        },
        {
            .Id = "text",
            .Name = "Text",
            .NewStage = [] { return new soil_samples::text::Stage(); },
        },
        {
            .Id = "line",
            .Name = "Line",
            .NewStage = [] { return new soil_samples::line::Stage(); },
        },
    };


    int stageIndex = -1;
    if (argc == 2) {
        stageIndex = parseIntOrDefault(argv[1], -1);
    }
    if (stageIndex == -1) {
        std::vector<soil_samples::gui::Stage::MenuItemDefinition> menuItems;
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
        auto backToDefault = [&engine] { engine.GetStageManager()->SetCurrent("default"); };
        for (const auto& option : stagesWithDesc) {
            if (option.Id == "default") {
                continue;
            }
            auto* stage = option.NewStage();
            engine.GetStageManager()->RegisterStage(option.Id, stage);
            stage->SetBackAction(backToDefault);
            menuItems.push_back({
                .Caption = option.Name,
                .Value = option.Id,
                .BackgroundTileName = "button",
                .IconTileName = "basic",
                .LetterSize = 0.5f,
                .OnClick =
                    [&](const soil_samples::gui::menu::Item& item) {
                        engine.GetStageManager()->SetCurrent(item.GetValue());
                    },
            });
        }

        defaultStage->Load();
        defaultStage->GenerateMenu(menuItems);
        engine.GetStageManager()->SetCurrent("default");
    } else {
        auto* stage = stagesWithDesc[stageIndex].NewStage();
        engine.GetStageManager()->RegisterStage(stagesWithDesc[stageIndex].Id, stage);
        engine.GetStageManager()->SetCurrent(stagesWithDesc[stageIndex].Id);
    }
    engine.Run();
    return 0;
}
