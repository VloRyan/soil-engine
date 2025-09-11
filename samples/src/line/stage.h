#ifndef LINE_STAGE_H
#define LINE_STAGE_H
#include <stage/stage.h>

#include "line_instance.h"

namespace soil_samples::line {
    constexpr int MAX_LINES = 32;

    class Stage final : public soil::stage::Stage {
    public:
        explicit Stage();
        ~Stage() override = default;
        void Handle(const soil::WindowEvent& event) override;
        void Load() override;
        void Update() override;


    private:
        void initInput(soil::stage::scene::Scene* scene);
        void initLines(soil::stage::scene::Scene* scene);
        std::array<LineInstance*, MAX_LINES> lines_;
        bool printStatistics_;
        int offset_;
    };
} // namespace soil_samples::line


#endif
