#ifndef INSTANCING_STAGE_H
#define INSTANCING_STAGE_H
#include <stage/stage.h>

#include "common/stage.h"
#include "shape_instance.h"

namespace soil_samples::instancing {
    constexpr int SHAPES_PER_DIM = 6;

    class Stage final : public common::Stage {
    public:
        explicit Stage();
        ~Stage() override = default;
        void Handle(const soil::WindowEvent& event) override;
        void OnLoad() override;

    protected:
        void RegisterInputEvents(soil::input::EventMap& eventMap) override;

    private:
        void initInput(soil::stage::scene::Scene* scene);
        static void initBackground(soil::stage::scene::Scene* scene, int textureIndex);
        void initCarrots(soil::stage::scene::Scene* scene, int textureIndex);
        std::array<ShapeInstance*, SHAPES_PER_DIM * SHAPES_PER_DIM> shapes_;
        bool printStatistics_;
    };
} // namespace soil_samples::instancing


#endif
