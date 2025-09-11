#ifndef SOIL_VIDEO_RENDER_STEP_RENDER_H
#define SOIL_VIDEO_RENDER_STEP_RENDER_H
#include "base.h"
#include "video/render/container.h"

namespace soil::video::render::step {
    class Render final : public Base {
    public:
        explicit Render(const std::string& id, StateDef stateDef = {});
        ~Render() override = default;
        void Process(Context& context) override;
        StateDef& GetStateDef();

    private:
        StateDef stateDef_;
        RenderDef renderDef_;
    };
} // namespace soil::video::render::step
#endif
