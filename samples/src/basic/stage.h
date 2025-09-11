#ifndef BASIC_STAGE_H
#define BASIC_STAGE_H
#include <stage/stage.h>

#include "engine.h"
#include "shape.h"

namespace soil_samples::basic {
    constexpr int SHAPES_PER_DIM = 8;

    class Stage final : public soil::stage::Stage {
    public:
        explicit Stage();
        ~Stage() override = default;
        void Handle(const soil::WindowEvent& event) override;
        void Load() override;

    private:
        void initInput(soil::stage::scene::Scene* scene);
        void initBackground(soil::stage::scene::Scene* scene, byte textureUnit) const;
        void initCarrots(soil::stage::scene::Scene* scene, byte textureUnit);
        std::array<Shape*, SHAPES_PER_DIM * SHAPES_PER_DIM> shapes_;
        bool printStatistics_;
    };
} // namespace soil_samples::basic


#endif
