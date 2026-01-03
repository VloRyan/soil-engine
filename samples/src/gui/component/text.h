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
                      const glm::vec3& worldPos,
                      soil::video::shader::Program* shader) override;

  void SetupText(soil::video::render::State& state) override;
  void SetupSymbol(const soil::stage::scene::component::text::Symbol* symbol,
                   const glm::vec3& worldPos,
                   soil::video::shader::Program* shader) override;

  inline static const auto CHARACTER_SHADER_NAME = std::string("Character");
  inline static const auto SYMBOL_SHADER_NAME = std::string("SymbolTile");
};
}  // namespace soil_samples::gui::component

#endif
