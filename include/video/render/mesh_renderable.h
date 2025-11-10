#ifndef SOIL_VIDEO_RENDER_MESH_RENDERABLE_H
#define SOIL_VIDEO_RENDER_MESH_RENDERABLE_H
#include "renderable.h"
#include "video/buffer/vbo.h"
#include "video/mesh/data.h"
#include "video/shader/shader.h"
#include "video/vertex/vao.h"
namespace soil::video::shader {
class Shader;
}
namespace soil::video::render {
class MeshRenderable : public Renderable {
 public:
  explicit MeshRenderable(const mesh::Data& mesh, shader::Shader* shader,
                          std::function<void(const data::IWriter& writer,
                                             soil::video::render::State& state)>
                              applyDataFunc);

  ~MeshRenderable() override;

  void Render(soil::video::render::State& state, int count) override;
  [[nodiscard]] const vertex::Vao* Vao() const override;
  [[nodiscard]] vertex::Vao* GetVao() const;
  [[nodiscard]] render::DrawMode DrawMode() const override;
  [[nodiscard]] RenderableGroup* NewGroup() const override;
  void ApplyData(const data::IWriter& writer,
                 soil::video::render::State& state) const override;
  void SetRenderFunc(
      const std::function<void(soil::video::render::State&, int)>& renderFunc);

 private:
  vertex::Vao* vao_;
  enum DrawMode drawMode_;
  std::function<void(const data::IWriter& writer,
                     soil::video::render::State& state)>
      applyDataFunc_;

  std::function<void(soil::video::render::State& state, int count)> renderFunc_;
};
}  // namespace soil::video::render

#endif
