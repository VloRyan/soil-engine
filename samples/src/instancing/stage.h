#ifndef INSTANCING_STAGE_H
#define INSTANCING_STAGE_H
#include <stage/stage.h>

#include "shape_instance.h"

namespace soil_samples::instancing {
    class Stage final : public soil::stage::Stage {
    public:
        explicit Stage(soil::stage::Manager* manager);
        ~Stage() override = default;
        void Handle(const soil::WindowEvent& event) override;

    private:
        std::array<ShapeInstance*, 4> shapes_;
        bool printStatistics_;
    };
} // namespace soil_samples::instancing


#endif // INSTANCING_STAGE_H
