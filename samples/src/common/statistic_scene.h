#ifndef SCENE_H
#define SCENE_H
#include <stage/scene/scene.h>

namespace soil_samples::common {
    constexpr auto UBO_TARGET_MATRICES = 0;

    class StatisticScene : public soil::stage::scene::Scene {
    public:
        explicit StatisticScene(soil::stage::Stage* stage);
        ~StatisticScene() override = default;
        void Handle(const soil::WindowEvent& event) override;
    };
} // namespace soil_samples::common

#endif // SCENE_H
