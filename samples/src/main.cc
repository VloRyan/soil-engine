
#include <iomanip>
#include <iostream>


#include "basic/shader.h"
#include "basic/stage.h"
#include "instancing/shader.h"
#include "instancing/stage.h"

int parseIntOrDefault(const std::string &s, const int defaultValue) {
    try {
        const int i{std::stoi(s)};
        return i;
    } catch (std::exception const &) {
        return defaultValue;
    }
}

int main(const int argc, const char *argv[]) {
    constexpr auto winParams = soil::WindowParameter{.Size = glm::ivec2(1920, 1080),
                                                     .RenderSize = glm::ivec2(1920, 1080),
                                                     .Type = soil::WindowType::Windowed,
                                                     .OpenGLVersion = glm::ivec2(3, 3)};
    const auto engine = soil::Engine(winParams);
    auto *vidMgr = engine.GetVideoManager();
    vidMgr->PrepareShader(new soil_samples::basic::Shader("Shader/"));
    vidMgr->PrepareShader(new soil_samples::instancing::Shader("Shader/"));

    std::vector<soil::stage::Stage *> stages;
    stages.push_back(new soil_samples::basic::Stage(engine.GetStageManager()));
    stages.push_back(new soil_samples::instancing::Stage(engine.GetStageManager()));

    int stageIndex = -1;
    if (argc == 2) {
        stageIndex = parseIntOrDefault(argv[1], -1);
    }
    if (stageIndex == -1) {
        std::cout << "Stages: \n"
                  << "(0): Basic\n"
                  << "(1): Instancing\n";
        int option = -1;
        while (stageIndex == -1) {
            std::cout << "Choose an option(0-" << stages.size() - 1 << "): ";
            std::cin >> option;
            if (option != -1) {
                stageIndex = option;
            }
        }
    }
    engine.GetStageManager()->SetCurrent(stages[stageIndex]);
    engine.Run();
    return 0;
}
