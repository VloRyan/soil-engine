#ifndef SOIL_STAGE_SCENE_COMPONENT_RENDER_MESH_INSTANCE_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_RENDER_MESH_INSTANCE_COMPONENT_H
#include "renderable_component.h"
#include "video/render/mesh_instance.h"
namespace soil::stage::scene::component::render {
class MeshInstanceComponent : public RenderableComponent {
 public:
  MeshInstanceComponent(
      const video::render::MeshInstancePile::PileDescriptor& pileDescriptor,
      bool opaque);

  ~MeshInstanceComponent() override;

  [[nodiscard]] video::render::Renderable* GetRenderable() override;

  virtual float DistanceTo(const glm::vec3& point);

  virtual void ApplyData(const video::render::data::IWriter& writer,
                         soil::video::render::State& state) = 0;
  void SetOpaque(bool opaque) override;

 protected:
  video::render::MeshInstance* instance_;
};
}  // namespace soil::stage::scene::component::render

#endif
