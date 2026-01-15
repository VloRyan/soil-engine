#ifndef SOIL_STAGE_SCENE_COMPONENT_RENDER_MESH_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_RENDER_MESH_COMPONENT_H
#include "stage/scene/component/render/drawable_component.h"
#include "video/render/draw/vao_elements.h"

namespace soil::stage::scene::component::render {
class MeshComponent : public DrawableComponent,
                      public video::render::draw::VaoElements::Data {
 public:
  MeshComponent(const video::vertex::Vao* vao, video::render::DrawMode mode,
                video::shader::Program* shader, bool opaque);

  ~MeshComponent() override;

  [[nodiscard]] virtual video::shader::Program* GetShader() const;

  [[nodiscard]] video::render::draw::Drawable* Drawable() override;

  void SetOpaque(bool opaque) override;
  float DistanceTo(const glm::vec3& point) override;

 protected:
  void SignalChanged(DrawableComponent::ChangeDetails detail) override;

 protected:
  video::render::draw::VaoElements* mesh_;
};
}  // namespace soil::stage::scene::component::render

#endif
