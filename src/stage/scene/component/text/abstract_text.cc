#include "stage/scene/component/text/abstract_text.h"

#include "stage/scene/gui/rectangle.h"
#include "stage/text/parser.h"

namespace soil::stage::scene::component::text {
std::unordered_map<std::string, AbstractText::PrefabData>
    AbstractText::PREFABS = {};

AbstractText::AbstractText(const std::string& prefab, const std::string& text)
    : data_(&PREFABS[prefab]),
      size_(glm::vec2(0.0F)),
      positionOffset_(0.F),
      characterOutline_(0.5F, 0.2F),
      borderOutline_(0.5F, 0.1F),
      characterSize_(1.0F),
      maxLineLength_(-1),
      color_(glm::vec4(1.0F)),
      borderColor_(0.F) {
  stateId_ = {
      .Shader = nullptr,  // no explicit
      .Vao = data_->QuadVao,
      .State =
          {
              .Blend = true,
              .DepthFunc = video::render::DepthFunc::LessEqual,
          },
  };
  AbstractText::SetText(text);
}

void AbstractText::InitPrefab(const std::string& name, const PrefabData& data) {
#ifdef DEBUG
  if (HasPrefab(name)) {
    throw std::runtime_error("prefab '" + name + "' already initialized");
  }
#endif
  PREFABS[name] = data;
}

void AbstractText::RemovePrefab(const std::string& name) {
  PREFABS.erase(name);
}

bool AbstractText::HasPrefab(const std::string& name) {
  return PREFABS.contains(name);
}

void AbstractText::SetText(const std::string& text) {
  if (text == text_) {
    return;
  }
  text_ = text;
  updateText();
}

std::string AbstractText::GetText() const { return text_; }

const std::vector<stage::text::Line>& AbstractText::GetLines() const {
  return lines_;
}

void AbstractText::SetPositionOffset(const glm::vec3& positionOffset) {
  positionOffset_ = positionOffset;
}

glm::vec3 AbstractText::GetPositionOffset() const { return positionOffset_; }

void AbstractText::updateText() {
  lines_ = stage::text::Parser::Parse(GetText(), data_->Font->Characters, data_->SymbolMap, maxLineLength_);
  const auto effectiveLineHeight =
      static_cast<float>(data_->Font->LineHeight - (data_->Font->Padding[0] + data_->Font->Padding[2]))
          * GetCharacterSize();
  size_ = glm::vec2(0.F, effectiveLineHeight * static_cast<float>(lines_.size()));
  for (const auto& line : lines_) {
    if (static_cast<float>(line.Length) * GetCharacterSize() > size_.x) {
      size_.x = static_cast<float>(line.Length) * GetCharacterSize();
    }
  }
  parseText();
}

glm::vec2 AbstractText::GetSize() const { return size_; }

void AbstractText::SetBorderOutline(const glm::vec2& borderOutline) {
  if (borderOutline == borderOutline_) {
    return;
  }
  borderOutline_ = borderOutline;
}

glm::vec2 AbstractText::GetBorderOutline() const { return borderOutline_; }

glm::vec2 AbstractText::GetCharacterOutline() const {
  return characterOutline_;
}

void AbstractText::SetCharacterOutline(const glm::vec2& characterOutline) {
  if (characterOutline == characterOutline_) {
    return;
  }
  characterOutline_ = characterOutline;
}

int AbstractText::GetMaxLineLength() const { return maxLineLength_; }

void AbstractText::SetMaxLineLength(int maxLineLength) {
  if (maxLineLength_ == maxLineLength) {
    return;
  }
  maxLineLength_ = maxLineLength;
  updateText();
}

byte AbstractText::GetTextureSlot() const {
  return data_->FontTexture->GetSlot();
}

glm::vec4 AbstractText::GetColor() const { return color_; }

void AbstractText::SetColor(const glm::vec4 color) {
  if (color == color_) {
    return;
  }
  color_ = color;
}

glm::vec3 AbstractText::GetBorderColor() const { return borderColor_; }

void AbstractText::SetBorderColor(const glm::vec3 color) {
  if (color == borderColor_) {
    return;
  }
  borderColor_ = color;
}

float AbstractText::GetCharacterSize() const { return characterSize_; }

void AbstractText::SetCharacterSize(const float size) {
  if (size == characterSize_) {
    return;
  }
  characterSize_ = size;
  updateText();
}

const file::Font* AbstractText::GetFont() const { return data_->Font; }

const AbstractText::PrefabData& AbstractText::Data() { return *data_; }

void AbstractText::Draw(video::render::State& state) {
  if (characterGlyphs_.empty() && symbolGlyphs_.empty()) {
    return;
  }
  auto parent = dynamic_cast<soil::stage::scene::gui::Rectangle*>(GetParent());
  state.Apply(stateId_.State);
  state.BindVao(stateId_.Vao);

  state.SetShader(data_->CharacterShader);
  data_->CharacterShader->Prepare(state);
  SetupText(state, parent);

  const auto parentPos = parent != nullptr ? parent->GetPosition() : glm::vec3(0);
  for (auto& data : characterGlyphs_) {
    DrawGlyph(data, parentPos);
  }
  if (symbolGlyphs_.empty() || data_->SymbolShader == nullptr) {
    return;
  }
  state.SetShader(data_->SymbolShader);
  data_->SymbolShader->Prepare(state);
  for (auto& data : symbolGlyphs_) {
    DrawGlyph(data, parentPos);
  }
}

void AbstractText::DrawGlyph(GlyphData& data, const glm::vec3& offset) {
  switch (data.Glyph->Type) {
    case stage::text::Glyph::Type::Character:
      SetupCharacter(*data.Glyph->Character,
                     data.Position + offset,
                     data.Color,
                     data_->CharacterShader);
      break;
    case stage::text::Glyph::Type::Symbol:
      SetupSymbol(*data.Glyph->Symbol,
                  data.Position + offset,
                  data.Color,
                  data_->SymbolShader);
    default:;
  }
  const auto* ebo = stateId_.Vao->GetEbo();
  soil::video::shader::Program::DrawElements(static_cast<uint>(video::render::DrawMode::TriangleStrip),
                                             ebo->GetIndexCount(),
                                             ebo->GetIndexType());
}

float AbstractText::DistanceTo(const glm::vec3& point) {
  return glm::distance(point.z, GetParent()->GetPosition().z + GetPositionOffset().z);  // sort by z
}

bool AbstractText::IsSortable() { return true; }

void AbstractText::UpdateState(const video::render::StateDef& state) {
  stateId_.State = state;
}

const video::render::StateIdentifier& AbstractText::StateId() const {
  return stateId_;
}

video::render::draw::Drawable* AbstractText::Drawable() { return this; }

std::unordered_map<std::string, stage::text::Symbol> AbstractText::MakeSymbolMap(const file::SpriteSheet& spriteSheet,
                                                                                 const file::Font& font,
                                                                                 const video::texture::Texture* symbolTexture) {
  auto map = std::unordered_map<std::string, stage::text::Symbol>();
  for (auto& pair : spriteSheet.Frames) {
    map.insert({pair.first,
                stage::text::Symbol{.Name = pair.first, .Texture = symbolTexture, .TileIndex = pair.second, .AdvanceX =
                font.Base + font.Padding[0] + font.Padding[2], .SizeX = font.Base,}});
  }
  return map;
}

void AbstractText::parseText() {

  glm::vec2 cursorPosition;
  const auto effectiveLineHeight = GetSize().y / static_cast<float>(GetLines().size());
  cursorPosition.y = effectiveLineHeight * static_cast<float>(GetLines().size() + 1) * 0.5F;

  int charsCount = 0;
  int symbolCount = 0;
  for (auto& line : GetLines()) {
    cursorPosition.x = GetSize().x * -0.5F;
    for (auto& word : line.Words) {
      for (auto& glyph : word.Glyphs) {
        switch (glyph.Type) {
          case stage::text::Glyph::Type::Character: {

            const glm::vec2 halfSize = (glm::vec2(glyph.Character->Size) * glm::vec2(0.5F)) * GetCharacterSize();
            const glm::vec2 fontOffset = glm::vec2(glyph.Character->Offset) * GetCharacterSize();

            const glm::vec2
                centerPosition(fontOffset.x + halfSize.x, effectiveLineHeight * -0.5F - (fontOffset.y + halfSize.y));

            const auto localPosition = cursorPosition + centerPosition + glm::vec2(GetPositionOffset());

            auto worldPos = glm::vec3(localPosition.x,
                                      localPosition.y,
                                      GetPositionOffset().z);
            if (characterGlyphs_.size() > charsCount) {
              characterGlyphs_[charsCount].Glyph = &glyph;
              characterGlyphs_[charsCount].Position = worldPos;
              characterGlyphs_[charsCount].Color = glyph.Color.value_or(GetColor());
            } else {
              characterGlyphs_.push_back(GlyphData{
                  .Glyph = &glyph,
                  .Position = worldPos,
                  .Color= glyph.Color.value_or(GetColor()),
              });
            }

            charsCount++;
            break;
          }
          case stage::text::Glyph::Type::Symbol: {
            auto halfSize = static_cast<float>(glyph.SizeX()) * 0.5F * GetCharacterSize();
            auto worldPos = GetPositionOffset() + glm::vec3(cursorPosition.x + halfSize,
                                                            cursorPosition.y - effectiveLineHeight * 0.5
                                                                - halfSize - data_->Font->Padding[1]
                                                                * GetCharacterSize(),
                                                            0.F);

            if (symbolGlyphs_.size() > symbolCount) {
              symbolGlyphs_[symbolCount].Glyph = &glyph;
              symbolGlyphs_[symbolCount].Position = worldPos;
              symbolGlyphs_[symbolCount].Color = glyph.Color.value_or(GetColor());
            } else {
              symbolGlyphs_.push_back(GlyphData{
                  .Glyph = &glyph,
                  .Position = worldPos,
                  .Color = glyph.Color.value_or(GetColor()),
              });
            }
            symbolCount++;

            break;
          }
          default:;
        }
        cursorPosition.x += static_cast<float>(glyph.AdvanceX()) * GetCharacterSize();
      }
    }
    cursorPosition.y -= effectiveLineHeight;
  }
  characterGlyphs_.resize(charsCount);
  symbolGlyphs_.resize(symbolCount);
}

}  // namespace soil::stage::scene::component::text

