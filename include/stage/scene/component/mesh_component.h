#ifndef SOIL_STAGE_SCENE_COMPONENT_MESH_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_MESH_COMPONENT_H
#include "stage/scene/component/render/renderable_component.h"
#include "video/render/mesh_renderable.h"

namespace soil::stage::scene::component {
class MeshComponent : public RenderableComponent {
 public:
  MeshComponent(const video::mesh::Data& mesh, video::shader::Shader* shader,
                bool opaque);

  ~MeshComponent() override;

  [[nodiscard]] virtual video::shader::Shader* GetShader() const;

  [[nodiscard]] video::render::Renderable* GetRenderable() override;

  virtual float DistanceTo(const glm::vec3& point);

  virtual void ApplyData(const video::render::data::IWriter& writer,
                         soil::video::render::State& state) = 0;
  void SetOpaque(bool opaque) override;

 protected:
  video::render::MeshRenderable* mesh_;
  video::shader::Shader* shader_;
};
}  // namespace soil::stage::scene::component

#endif
