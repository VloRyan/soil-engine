#ifndef SOIL_STAGE_SCENE_COMPONENT_MESH_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_MESH_COMPONENT_H
#include "video/render/vertex_array.h"
#include "visual_component.h"

namespace soil::stage::scene::component {
class MeshComponent : public VisualComponent,
                      public video::render::RenderableObject {
 public:
  MeshComponent(const video::mesh::Data& mesh, video::shader::Shader* shader,
                bool opaque);

  ~MeshComponent() override;

  [[nodiscard]] virtual video::shader::Shader* GetShader() const;

  [[nodiscard]] RenderableObject* GetRenderable() override;

  void Render(video::render::State& state) override;

  float DistanceTo(const glm::vec3& point) override;

 protected:
  virtual void BuildVaoFrom(const video::mesh::Data& mesh);

  virtual void PrepareRender(video::render::State& state) {}

  [[nodiscard]] virtual video::render::VertexArray* GetVertexArray() const;

 private:
  video::render::VertexArray* vertexArray_;
  video::shader::Shader* shader_;
};
}  // namespace soil::stage::scene::component

#endif
