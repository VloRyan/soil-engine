#ifndef SOIL_VIDEO_RENDER_MESH_INSTANCE_H
#define SOIL_VIDEO_RENDER_MESH_INSTANCE_H
#include "mesh_instance_pile.h"
#include "mesh_renderable.h"
namespace soil::video::shader {
class Shader;
}
namespace soil::video::render {
class MeshInstance : public Renderable {
 public:
  explicit MeshInstance(const MeshInstancePile::PileDescriptor& pileDescriptor,
                        std::function<void(const data::IWriter& writer,
                                           soil::video::render::State& state)>
                            applyDataFunc);

  ~MeshInstance() override = default;

  void Render(soil::video::render::State& state, int count) override;
  [[nodiscard]] const vertex::Vao* Vao() const override;
  [[nodiscard]] render::DrawMode DrawMode() const override;
  [[nodiscard]] RenderableGroup* NewGroup() const override;
  void ApplyData(const data::IWriter& writer,
                 soil::video::render::State& state) const override;

 private:
  const MeshInstancePile::PileDescriptor& pileDescriptor_;
  std::function<void(const data::IWriter& writer,
                     soil::video::render::State& state)>
      applyDataFunc_;
};
}  // namespace soil::video::render
#endif
