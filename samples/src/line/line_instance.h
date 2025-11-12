#ifndef LINE_LINE_INSTANCE_H
#define LINE_LINE_INSTANCE_H
#include <vector>

#include "stage/scene/component/render/mesh_instance_component.h"
#include "video/vertex/vertex.h"
namespace soil_samples::line {

class LineInstance final
    : public soil::stage::scene::component::render::MeshInstanceComponent {
 public:
  struct Data {
    glm::vec3 Start;
    glm::vec3 End;
    glm::vec4 Color;
  };

  LineInstance(const soil::video::render::MeshInstancePile::PileDescriptor&
                   pileDescriptor,
               glm::vec3 StartPoint, glm::vec3 EndPoint);

  static LineInstance* NewFromPile(const std::string& name,
                                   glm::vec3 StartPoint, glm::vec3 EndPoint);

  [[nodiscard]] glm::vec4 GetColor() const;

  void SetColor(glm::vec4 color);

  [[nodiscard]] float GetLength() const;

  void SetStartPoint(glm::vec3 StartPoint);

  void SetEndPoint(glm::vec3 EndPoint);

  [[nodiscard]] glm::vec3 GetEndPoint() const;

  void ApplyData(const soil::video::render::data::IWriter& writer,
                 soil::video::render::State& state) override;

  void Update() override;
  inline static const auto BATCH_NAME = std::string("Line");
  static std::vector<soil::video::vertex::VertexAttribDescriptor> ATTRIBS;

 private:
  Data data_;
  glm::vec3 localStartPoint_;
  glm::vec3 localEndPoint_;
};
}  // namespace soil_samples::line

#endif
