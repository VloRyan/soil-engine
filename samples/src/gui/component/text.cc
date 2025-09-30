#include "text.h"

#include <bits/fs_fwd.h>

#include "stage/scene/gui/rectangle.h"
#include "stage/scene/scene.h"
#include "stage/scene/text/parser.h"

namespace soil_samples::gui::component {
Text::Text(const std::string& prefab, const std::string& text)
    : AbstractText(prefab, text) {}

void Text::Render(soil::video::render::State& state) {
  const auto shader = GetShader();
  shader->Prepare(state);
  const auto prevDepthFunc = state.GetDepthFunc();
  state.SetDepthFunc(soil::video::render::DepthFunc::LessEqual);
  glm::vec2 cursorPosition;
  const auto effectiveLineHeight =
      GetSize().y / static_cast<float>(GetLines().size());
  cursorPosition.y =
      effectiveLineHeight * static_cast<float>(GetLines().size() + 1) * 0.5F;

  const auto* parentRect =
      dynamic_cast<soil::stage::scene::gui::Rectangle*>(GetParent());
  if (parentRect != nullptr) {
    state.SetScissorTest(true);
    state.SetScissor(parentRect->GetScissorRect());
  } else {
    state.SetScissorTest(false);
  }

  shader->SetUniform("uTexture", GetTextureSlot());

  for (auto& line : GetLines()) {
    cursorPosition.x = GetSize().x * -0.5F;

    for (auto& word : line.Words) {
      for (auto i = 0; i < word.Characters.size(); ++i) {
        const auto& character = *word.Characters[i];
        const glm::vec2 halfSize =
            (glm::vec2(character.Size) * glm::vec2(0.5F)) * GetCharacterSize();
        const glm::vec2 fontOffset =
            glm::vec2(character.Offset) * GetCharacterSize();

        const glm::vec2 centerPosition(
            fontOffset.x + halfSize.x,
            effectiveLineHeight * -0.5F - (fontOffset.y + halfSize.y));

        const auto localPosition =
            cursorPosition + centerPosition + glm::vec2(GetPositionOffset());

        const auto parentPos = GetParent()->GetWorldPosition();
        shader->SetUniform("uPositionWorldspace",
                           glm::vec3(localPosition.x + parentPos.x,
                                     localPosition.y + parentPos.y,
                                     parentPos.z + GetPositionOffset().z));

        shader->SetUniform("uSize",
                           glm::vec2(character.Size) * GetCharacterSize());
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
        GetVertexArray()->Render(state);

        const auto advance =
            static_cast<float>(character.XAdvance) * GetCharacterSize();
        cursorPosition.x += advance;
      }
    }
    cursorPosition.y -= effectiveLineHeight;
  }
  state.SetDepthFunc(prevDepthFunc);
}

float Text::DistanceTo(const glm::vec3& point) {
  return glm::distance(
      GetParent()->GetWorldPosition().z + GetPositionOffset().z, point.z);
}

}  // namespace soil_samples::gui::component
