#ifndef SOIL_VIDEO_RENDER_BATCH_H
#define SOIL_VIDEO_RENDER_BATCH_H

#include "instance/buffer.h"
#include "renderable.h"
#include "state.h"
#include "vertex_array.h"
#include "video/mesh/data.h"
#include "video/vertex/vao.h"

namespace soil::video::render {
struct BatchDescriptor {
  video::mesh::Data* MeshData{nullptr};
  shader::Shader* Shader{nullptr};
  int InstanceSize{0};
  int PreserveInstances{100};
  shader::DrawMode DrawMode{shader::DrawMode::Triangles};
  std::vector<vertex::VertexAttribDescriptor> VertexAttribDescriptors{};
  StateDef State{};
};

class Batch : public Renderable {
 public:
  explicit Batch(const BatchDescriptor& descriptor);

  ~Batch() override;

  void AddChangedInstance(instance::Instance* instance) const;

  void AddNewInstance(instance::Instance* instance) const;

  bool RemoveInstance(instance::Instance* instance) const;

  void Update() const;

  void Render(State& state) override;

  [[nodiscard]] virtual shader::DrawMode GetDrawMode() const;

  [[nodiscard]] virtual shader::Shader* GetShader() const;

 protected:
  explicit Batch();  // unit testing

 private:
  buffer::Object* instanceVbo_;
  instance::Buffer* instanceBuffer_;
  shader::Shader* shader_;
  VertexArray* va_;
  shader::DrawMode drawMode_;
  StateDef stateDef_;
};
}  // namespace soil::video::render

#endif
