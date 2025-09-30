#include "video/render/step/render.h"

namespace soil::video::render::step {
Render::Render(const std::string& id, const StateDef stateDef)
    : Base(id, Type::Render), stateDef_(stateDef) {}

void Render::Process(Context& context) {
  for (auto* renderable : *context.Renderables) {
    context.State->Apply(stateDef_);
    renderable->Render(*context.State);
  }
}

StateDef& Render::GetStateDef() { return stateDef_; }
}  // namespace soil::video::render::step
