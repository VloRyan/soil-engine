#include "video/render/mesh_instance.h"

#include <utility>

#include "video/render/renderable_group_instances.h"
namespace soil::video::render {

MeshInstance::MeshInstance(
    const MeshInstancePile::PileDescriptor &pileDescriptor,
    std::function<void(const data::IWriter &, soil::video::render::State &)>
        applyDataFunc)
    : Renderable({
          .Shader = pileDescriptor.Shader,
          .DrawMode = pileDescriptor.MeshData->GetDrawMode(),
      }),
      pileDescriptor_(pileDescriptor),
      applyDataFunc_(std::move(applyDataFunc)) {}

void MeshInstance::Render(State &state, int count) {}

const vertex::Vao *MeshInstance::Vao() const { return nullptr; }

render::DrawMode MeshInstance::DrawMode() const {
  return pileDescriptor_.MeshData->GetDrawMode();
}
RenderableGroup *MeshInstance::NewGroup() const {
  return new RenderableGroupInstances(pileDescriptor_);
}
void MeshInstance::ApplyData(const data::IWriter &writer, State &state) const {
  applyDataFunc_(writer, state);
}

}  // namespace soil::video::render
