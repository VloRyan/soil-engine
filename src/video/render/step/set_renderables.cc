#include "video/render/step/set_renderables.h"

namespace soil::video::render::step {
SetRenderables::SetRenderables(const std::string& id, const RenderDef& byDef,
                               Base* requiredStep)
    : Base(id, Type::Prepare, requiredStep), byDef_(byDef) {}

void SetRenderables::Process(Context& context) {
  renderablesBuffer_.clear();
  context.RenderContainer->GetPerDef(renderablesBuffer_, byDef_);
  context.Renderables = &renderablesBuffer_;
  context.RenderablesState = byDef_;
}
}  // namespace soil::video::render::step
