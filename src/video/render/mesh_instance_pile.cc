#include "video/render/mesh_instance_pile.h"

#include "video/render/instance/buffer.h"
namespace soil::video::render {
std::unordered_map<std::string, MeshInstancePile::PileDescriptor>
    MeshInstancePile::PILE_MAP_ =
        std::unordered_map<std::string, MeshInstancePile::PileDescriptor>();

void MeshInstancePile::RegisterPile(
    std::string name, const MeshInstancePile::PileDescriptor &descriptor) {
  PILE_MAP_.insert({name, descriptor});
}

const MeshInstancePile::PileDescriptor *MeshInstancePile::GetPileDescription(
    const std::string &name) {
  auto itr = PILE_MAP_.find(name);
  if (itr == PILE_MAP_.end()) {
    return nullptr;
  }
  return &itr->second;
}

GLsizei MeshInstancePile::InstanceSize(
    const std::vector<vertex::VertexAttribDescriptor> &attribs) {
  GLsizei instanceSize = 0;
  for (const auto [Elements, Type] : attribs) {
    instanceSize +=
        video::vertex::AttributePointer::GetSizeOfDataType(Type) * Elements;
  }
  return instanceSize;
}
}  // namespace soil::video::render
