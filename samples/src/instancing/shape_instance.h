#ifndef INSTANCING_SHAPE_INSTANCE_H
#define INSTANCING_SHAPE_INSTANCE_H
#include <stage/scene/component/instance_data.h>

#include <vector>

#include "stage/scene/component/render/mesh_instance_component.h"
#include "video/vertex/vertex.h"

namespace soil_samples::instancing {
class ShapeInstance
    : public soil::stage::scene::component::render::MeshInstanceComponent {
 public:
  struct Data {
    glm::mat4 Matrix{1.F};
    glm::vec2 Size{1.F};
    glm::vec4 Color{1.F};
    uint TextureIndex{0};
  };

  explicit ShapeInstance(
      const soil::video::render::MeshInstancePile::PileDescriptor&
          pileDescriptor,
      bool isOpaque = true);

  ~ShapeInstance() override = default;

  static ShapeInstance* NewFromPile(const std::string& name = "shape",
                                    bool isOpaque = true);

  [[nodiscard]] virtual uint GetTextureIndex() const;
  virtual void SetTextureIndex(uint index);
  [[nodiscard]] virtual glm::vec2 GetSize() const;
  virtual void SetSize(const glm::vec2& size);
  [[nodiscard]] virtual glm::vec4 GetColor() const;
  virtual void SetColor(const glm::vec4& color);

  static std::vector<soil::video::vertex::VertexAttribDescriptor> ATTRIBS;
  static inline std::string BATCH_NAME = "Object";
  void Update() override;

 protected:
  void ApplyData(const soil::video::render::data::IWriter& writer,
                 soil::video::render::State& state) override;

 private:
  Data data_;
};
}  // namespace soil_samples::instancing

#endif
