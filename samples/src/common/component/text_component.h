#ifndef SOIL_EXAMPLES_COMMON_COMPONENT_TEXT_H
#define SOIL_EXAMPLES_COMMON_COMPONENT_TEXT_H
#include "stage/scene/component/text/abstract_text.h"
#include "stage/scene/node.h"
#include "stage/text/parser.h"
#include "video/texture/texture.h"

namespace soil_samples::common::component {
class TextComponent : public soil::stage::scene::component::text::AbstractText {
 public:
  explicit TextComponent(const std::string& prefab,
                         const std::string& text = "");
  ~TextComponent() override = default;
  void SetupCharacter(const soil::file::Font::Character& character,
                      const glm::vec3& worldPos, const glm::vec4& color,
                      soil::video::shader::Program* shader) override;

  void SetupText(soil::video::render::State& state,
                 soil::stage::scene::gui::Rectangle* parentRect) override;
  void SetupSymbol(const soil::stage::text::Symbol& symbol,
                   const glm::vec3& worldPos, const glm::vec4& color,
                   soil::video::shader::Program* shader) override;

  inline static const auto CHARACTER_SHADER_NAME = std::string("Character");
  inline static const auto SYMBOL_SHADER_NAME = std::string("SymbolTile");
};
}  // namespace soil_samples::common::component

#endif
