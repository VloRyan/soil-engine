#ifndef SOIL_VIDEO_RENDER_RENDERABLE_GROUP_INSTANCES_H
#define SOIL_VIDEO_RENDER_RENDERABLE_GROUP_INSTANCES_H
#include "mesh_instance.h"
#include "renderable_group.h"
#include "vector"
#include "video/render/data/buffer_writer.h"
#include "video/render/instance/buffer.h"

namespace soil::video::render {
class RenderableGroupInstances : public RenderableGroup {
 public:
  explicit RenderableGroupInstances(
      const MeshInstancePile::PileDescriptor& pileDescriptor);
  ~RenderableGroupInstances() override;
  void Insert(Renderable* renderable) override;
  bool Remove(Renderable* renderable) override;
  bool Empty() const override;
  const std::vector<Renderable*>& Content() const override;
  void Render(video::render::State& state) override;

 private:
  void Update();
  static GLsizei InstanceSize(
      const std::vector<vertex::VertexAttribDescriptor>& attribs);

  [[nodiscard]] size_t GetInstancesCount() const;
  [[nodiscard]] size_t GetDirtyInstancesCount() const;

  static constexpr auto UnsetId = -1;
  bool isDirty(const Renderable* instance);
  std::vector<size_t> dirtyInstanceIndices_;
  std::vector<int> instanceIndices_;
  std::vector<MeshInstance*> instances_;

  buffer::Object* instanceBuffer_;
  size_t instanceSize_;
  data::BufferWriter* dataWriter_;
  bool dirty_;
  MeshRenderable* mesh_;
};
}  // namespace soil::video::render
#endif
