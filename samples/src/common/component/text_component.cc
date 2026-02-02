#include "text_component.h"

#include <bits/fs_fwd.h>

#include "stage/scene/gui/rectangle.h"
#include "stage/text/parser.h"

namespace soil_samples::common::component {
TextComponent::TextComponent(const std::string& prefab, const std::string& text)
    : AbstractText(prefab, text) {}

void TextComponent::SetupText(soil::video::render::State& state,
                              soil::stage::scene::gui::Rectangle* parentRect) {
  if (parentRect != nullptr) {
    state.SetScissorTest(true);
    state.SetScissor(parentRect->GetScissorRect());
  } else {
    state.SetScissorTest(false);
  }
  data_->CharacterShader->SetUniform("uTexture", GetTextureSlot());
}

void TextComponent::SetupCharacter(const soil::file::Font::Character& character,
                                   const glm::vec3& worldPos,
                                   const glm::vec4& color,
                                   soil::video::shader::Program* shader) {
  shader->SetUniform("uPositionWorldSpace", worldPos);

  shader->SetUniform("uSize", glm::vec2(character.Size) * GetCharacterSize());
  const auto texCoords =
      glm::vec2(character.TextureCoord.x,
                GetFont()->ImageSize.y - character.TextureCoord.y) /
      glm::vec2(GetFont()->ImageSize);
  const auto relSize =
      glm::vec2(character.Size) / glm::vec2(GetFont()->ImageSize);
  shader->SetUniform("uTexCoords", texCoords);
  shader->SetUniform("uTexCoordsMax", glm::vec2(texCoords.x + relSize.x,
                                                texCoords.y - relSize.y));

  shader->SetUniform("uColor", color);
  shader->SetUniform("uBorderColor", GetBorderColor());
  shader->SetUniform("uBorderOutline", GetBorderOutline());
  shader->SetUniform("uCharacterOutline", GetCharacterOutline());
}
void TextComponent::SetupSymbol(const soil::stage::text::Symbol& symbol,
                                const glm::vec3& worldPos,
                                const glm::vec4& color,
                                soil::video::shader::Program* shader) {
  shader->SetUniform("uPositionWorldSpace", worldPos);
  shader->SetUniform("uSize", glm::vec2(symbol.SizeX) * GetCharacterSize());
  shader->SetUniform("uColor", color);
  shader->SetUniform("uTexture", symbol.Texture->GetSlot());
  shader->SetUniform("uTileIndex", symbol.TileIndex);
}

}  // namespace soil_samples::common::component
