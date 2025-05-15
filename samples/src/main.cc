
#include "basic/shader.h"
#include "basic/stage.h"
#include "instancing/shader.h"
#include "instancing/stage.h"

int main(const int argc, const char *argv[]) {
    constexpr auto winParams = soil::WindowParameter{.Size = glm::ivec2(1920, 1080),
                                                     .RenderSize = glm::ivec2(1920, 1080),
                                                     .Type = soil::WindowType::Windowed,
                                                     .OpenGLVersion = glm::ivec2(3, 3)};
    const auto engine = soil::Engine(winParams);
    auto *vidMgr = engine.GetVideoManager();
    vidMgr->PrepareShader(new soil_samples::basic::Shader("Shader/"));
    vidMgr->PrepareShader(new soil_samples::instancing::Shader("Shader/"));

    auto *basic = new soil_samples::basic::Stage(engine.GetStageManager());
    auto *instancing = new soil_samples::instancing::Stage(engine.GetStageManager());

    engine.GetStageManager()->SetCurrent(instancing);
    engine.Run();
    return 0;
}
