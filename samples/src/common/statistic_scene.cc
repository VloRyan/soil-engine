#include "statistic_scene.h"
#include "plog/Log.h"

namespace soil_samples::common {
    StatisticScene::StatisticScene(soil::stage::Stage* stage) : Scene(stage) {}

    void StatisticScene::Handle(const soil::WindowEvent& event) {
        if (event.GetCause() == soil::WindowEvent::StatisticsChanged) {
            const auto stats = event.GetWindow()->GetStatistics();
            PLOG_DEBUG << "FPS: " << std::to_string(stats.fps) << " Draws: " << std::to_string(stats.drawCount)
                       << " Vertices: " << std::to_string(stats.vertexCount);
        }
        Scene::Handle(event);
    }
} // namespace soil_samples::common
