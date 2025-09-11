#ifndef VOLUME_STAGE_H
#define VOLUME_STAGE_H
#include "basic/shader.h"
#include "basic/shape.h"
#include "engine.h"
#include "stage/scene/volume/container.h"
#include "stage/stage.h"

namespace soil_samples::volume {
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
        std::array<basic::Shape*, 4> shapes_;
        bool printStatistics_;
        soil::stage::scene::volume::Container* container_;
    };
} // namespace soil_samples::volume


#endif
