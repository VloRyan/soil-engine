#ifndef SOIL_VIDEO_RENDER_STEP_SET_RENDERABLES_H
#define SOIL_VIDEO_RENDER_STEP_SET_RENDERABLES_H
#include "base.h"

namespace soil::video::render::step {
    class SetRenderables : public Base {
    public:
        SetRenderables(const std::string& id, const RenderDef& byDef, Base* requiredStep = nullptr);
        ~SetRenderables() override = default;
        void Process(Context& context) override;

    private:
        RenderDef byDef_;
        std::vector<RenderableObject*> renderablesBuffer_;
    };
} // namespace soil::video::render::step


#endif
