
#ifndef SOIL_VIDEO_RENDER_MESH_INSTANCE_PILE_H
#define SOIL_VIDEO_RENDER_MESH_INSTANCE_PILE_H
#include "mesh_renderable.h"

namespace soil::video::render {
class MeshInstancePile final {
 public:
  struct PileDescriptor {
    video::mesh::Data* MeshData{nullptr};
    shader::Shader* Shader{nullptr};
    std::vector<vertex::VertexAttribDescriptor> VertexAttribDescriptors{};
    int PreserveInstances{100};
  };
  MeshInstancePile() = default;
  virtual ~MeshInstancePile() = default;
  static void RegisterPile(std::string name,
                           const MeshInstancePile::PileDescriptor& descriptor);
  static const MeshInstancePile::PileDescriptor* GetPileDescription(
      const std::string& name);

 private:
  static GLsizei InstanceSize(
      const std::vector<vertex::VertexAttribDescriptor>& attribs);
  static std::unordered_map<std::string, PileDescriptor> PILE_MAP_;
};
}  // namespace soil::video::render
#endif
