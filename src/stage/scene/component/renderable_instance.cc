
#include "stage/scene/component/renderable_instance.h"

namespace stage::scene::component {
RenderableInstance::RenderableInstance(video::mesh::Mesh *mesh,
                                       video::shader::InstanceShader *shader,
                                       const bool opaque)
    : mesh_(mesh), shader_(shader), opaque_(opaque) {}

video::mesh::Mesh *RenderableInstance::GetMesh() const { return mesh_; }

video::shader::InstanceShader *RenderableInstance::GetShader() const {
  return shader_;
}

bool RenderableInstance::IsOpaque() const { return opaque_; }

std::string RenderableInstance::GetBatchKey() const { return batchKey_; }

void RenderableInstance::SetBatchKey(const std::string &batchKey) {
  batchKey_ = batchKey;
}

void RenderableInstance::SetOpaque(const bool opaque) {
  /*if (opaque_ == opaque) {
      return;
  }*/
  opaque_ = opaque;
  batchKey_ = "";
}
} // namespace stage::scene::component
