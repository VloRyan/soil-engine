#include "text.h"

#include <bits/fs_fwd.h>

#include "stage/scene/gui/rectangle.h"
#include "stage/scene/text/parser.h"

namespace soil_samples::gui::component {
Text::Text(const std::string& prefab, const std::string& text)
    : AbstractText(prefab, text) {}

void Text::SetupText(soil::video::render::State& state) {
  const auto* parentRect =
      dynamic_cast<soil::stage::scene::gui::Rectangle*>(GetParent());
  if (parentRect != nullptr) {
    state.SetScissorTest(true);
    state.SetScissor(parentRect->GetScissorRect());
  } else {
    state.SetScissorTest(false);
  }
  data_->CharacterShader->SetUniform("uTexture", GetTextureSlot());
}

void Text::SetupCharacter(const soil::file::Font::Character& character,
                          const glm::vec3& worldPos,
                          soil::video::shader::Program* shader) {
  shader->SetUniform("uPositionWorldspace", worldPos);

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

  shader->SetUniform("uColor", GetColor());
  shader->SetUniform("uBorderColor", GetBorderColor());
  shader->SetUniform("uBorderOutline", GetBorderOutline());
  shader->SetUniform("uCharacterOutline", GetCharacterOutline());
}
void Text::SetupSymbol(
    const soil::stage::scene::component::text::Symbol* symbol,
    const glm::vec3& worldPos, soil::video::shader::Program* shader) {
  shader->SetUniform("uPositionWorldSpace", worldPos);
  shader->SetUniform("uSize", glm::vec2(symbol->SizeX) * GetCharacterSize());
  // shader->SetUniform("uColor", GetColor());
  shader->SetUniform("uColor", glm::vec4(1.F));
  shader->SetUniform("uTexture", symbol->Texture->GetSlot());
  shader->SetUniform("uTileIndex", symbol->TileIndex);
}

}  // namespace soil_samples::gui::component
