#include "video/render/processing/context.h"

namespace video::render {
Context::Context() : properties_(nullptr), technique_(nullptr), state_() {}

Properties *Context::GetProperties() const { return properties_; }

void Context::SetProperties(Properties *properties) {
  properties_ = properties;
}

Technique *Context::GetTechnique() const { return technique_; }

void Context::SetTechnique(Technique *algorithm) { technique_ = algorithm; }

State &Context::GetState() const { return *state_; }

void Context::SetState(State &state) { state_ = &state; }
} // namespace video::render
