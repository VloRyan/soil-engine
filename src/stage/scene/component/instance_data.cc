#include "stage/scene/component/instance_data.h"

#include <utility>

namespace soil::stage::scene::component {
InstanceData::InstanceData(std::string batchKey, const bool opaque)
    : VisualComponent(render::Type::Instancing, opaque),
      batchKey_(std::move(batchKey)) {}

std::string InstanceData::GetBatchKey() { return batchKey_; }

video::render::RenderableObject* InstanceData::GetRenderable() {
  return nullptr;
}
}  // namespace soil::stage::scene::component
