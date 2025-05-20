#ifndef BASIC_STAGE_H
#define BASIC_STAGE_H
#include <stage/stage.h>


#include "engine.h"
#include "shape.h"

namespace soil_samples::basic {
    class Stage final : public soil::stage::Stage {
    public:
        explicit Stage(soil::stage::Manager* manager);
        ~Stage() override = default;
        void Handle(const soil::WindowEvent& event) override;

    private:
        void initInput(soil::stage::scene::Scene* scene);
        void initBackground(soil::stage::scene::Scene* scene, const std::string& textureName) const;
        void initCarrots(soil::stage::scene::Scene* scene, const std::string& textureName);
        std::array<Shape*, 4> shapes_;
        bool printStatistics_;
    };
} // namespace soil_samples::basic


#endif // BASIC_STAGE_H
