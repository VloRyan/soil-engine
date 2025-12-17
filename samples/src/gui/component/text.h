#ifndef GUI_TEXT_H
#define GUI_TEXT_H
#include "stage/scene/component/text/abstract_text.h"
#include "stage/scene/node.h"
#include "stage/scene/text/parser.h"
#include "video/texture/texture.h"

namespace soil_samples::gui::component {
class Text : public soil::stage::scene::component::text::AbstractText {
 public:
  explicit Text(const std::string& prefab, const std::string& text = "");
  ~Text() override = default;
  void SetupCharacter(const soil::file::Font::Character& character,
                      const glm::vec3& worldPos) override;
  void Bind(soil::video::render::State& state) override;
  // void Render(soil::video::render::State& state) override;

  // void Apply(soil::video::render::State& state) override;
  // float Ordinal(const soil::video::render::State& state) override;
  /*void Draw(soil::video::render::State& state,
            soil::video::render::DrawMode mode, int indexCount,
            soil::video::vertex::IndexType indexType) override;*/
};
}  // namespace soil_samples::gui::component

#endif
