#include "video/render/step/render.h"

namespace soil::video::render::step {
    Render::Render(const std::string &id, const RenderDef renderDef, const StateDef stateDef) :
        Base(id), stateDef_(stateDef), renderDef_(renderDef) {}

    void Render::Process(Context &context) {
        context.State->Apply(stateDef_);
        const auto &renderables = context.Container->GetPerDef(renderDef_);
        for (const auto &renderable : renderables) {
            renderable->Render();
        }
    }
} // namespace soil::video::render::step
