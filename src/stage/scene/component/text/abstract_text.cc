#include "stage/scene/component/text/abstract_text.h"

#include "stage/scene/component/text/parser.h"
#include "stage/scene/gui/rectangle.h"

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

const std::vector<Line>& AbstractText::GetLines() const { return lines_; }

void AbstractText::SetPositionOffset(const glm::vec3& positionOffset) {
  positionOffset_ = positionOffset;
  SignalChanged();
}

glm::vec3 AbstractText::GetPositionOffset() const { return positionOffset_; }

void AbstractText::updateText() {
  lines_ = Parser::Parse(GetText(), data_->Font->Characters, data_->SymbolMap,
                         maxLineLength_);
  const auto effectiveLineHeight =
      static_cast<float>(data_->Font->LineHeight -
                         (data_->Font->Padding[0] + data_->Font->Padding[2])) *
      GetCharacterSize();
  size_ =
      glm::vec2(0.F, effectiveLineHeight * static_cast<float>(lines_.size()));
  for (const auto& line : lines_) {
    if (static_cast<float>(line.Length) * GetCharacterSize() > size_.x) {
      size_.x = static_cast<float>(line.Length) * GetCharacterSize();
    }
  }
  SignalChanged();
}

glm::vec2 AbstractText::GetSize() const { return size_; }

void AbstractText::SetBorderOutline(const glm::vec2& borderOutline) {
  if (borderOutline == borderOutline_) {
    return;
  }
  borderOutline_ = borderOutline;
  SignalChanged();
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
  SignalChanged();
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
  SignalChanged();
}

glm::vec3 AbstractText::GetBorderColor() const { return borderColor_; }

void AbstractText::SetBorderColor(const glm::vec3 color) {
  if (color == borderColor_) {
    return;
  }
  borderColor_ = color;
  SignalChanged();
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
  state.Apply(stateId_.State);
  state.BindVao(stateId_.Vao);

  state.SetShader(data_->CharacterShader);
  data_->CharacterShader->Prepare(state);
  SetupText(state);
  std::vector<SymbolPosition> symbolPositions;
  glm::vec2 cursorPosition;
  const auto effectiveLineHeight =
      GetSize().y / static_cast<float>(GetLines().size());
  cursorPosition.y =
      effectiveLineHeight * static_cast<float>(GetLines().size() + 1) * 0.5F;
  const auto parentPos = GetParent()->GetPosition();

  for (auto& line : GetLines()) {
    cursorPosition.x = GetSize().x * -0.5F;

    for (auto& word : line.Words) {
      for (auto glyph : word.Glyphs) {
        switch (glyph.Type) {
          case Glyph::Type::Character: {
            const glm::vec2 halfSize =
                (glm::vec2(glyph.Character->Size) * glm::vec2(0.5F)) *
                GetCharacterSize();
            const glm::vec2 fontOffset =
                glm::vec2(glyph.Character->Offset) * GetCharacterSize();

            const glm::vec2 centerPosition(
                fontOffset.x + halfSize.x,
                effectiveLineHeight * -0.5F - (fontOffset.y + halfSize.y));

            const auto localPosition = cursorPosition + centerPosition +
                                       glm::vec2(GetPositionOffset());

            const auto worldPos = glm::vec3(
                localPosition.x + parentPos.x, localPosition.y + parentPos.y,
                parentPos.z + GetPositionOffset().z);
            cursorPosition.x += DrawCharacter(worldPos, *glyph.Character);
            break;
          }
          case Glyph::Type::Symbol: {
            auto halfSize =
                static_cast<float>(glyph.SizeX()) * 0.5F * GetCharacterSize();
            auto worldPos =
                parentPos +
                glm::vec3(cursorPosition.x + halfSize,
                          cursorPosition.y - effectiveLineHeight * 0.5 -
                              halfSize -
                              data_->Font->Padding[1] * GetCharacterSize(),
                          0.F);
            symbolPositions.push_back(
                SymbolPosition{.Symbol = glyph.Symbol, .Position = worldPos});
            cursorPosition.x +=
                static_cast<float>(glyph.AdvanceX()) * GetCharacterSize();
            break;
          }
          default:;
        }
      }
    }
    cursorPosition.y -= effectiveLineHeight;
  }
  if (symbolPositions.empty() || data_->SymbolShader == nullptr) {
    return;
  }
  state.SetShader(data_->SymbolShader);
  data_->SymbolShader->Prepare(state);
  for (auto& symPos : symbolPositions) {
    SetupSymbol(symPos.Symbol, symPos.Position, data_->SymbolShader);
    const auto* ebo = stateId_.Vao->GetEbo();
    soil::video::shader::Program::DrawElements(
        static_cast<uint>(video::render::DrawMode::TriangleStrip),
        ebo->GetIndexCount(), ebo->GetIndexType());
  }
}
float AbstractText::DrawCharacter(const glm::vec3& at,
                                  const file::Font::Character& character) {
  SetupCharacter(character, at, data_->CharacterShader);
  const auto* ebo = stateId_.Vao->GetEbo();
  soil::video::shader::Program::DrawElements(
      static_cast<uint>(video::render::DrawMode::TriangleStrip),
      ebo->GetIndexCount(), ebo->GetIndexType());

  return static_cast<float>(character.AdvanceX) * GetCharacterSize();
}

float AbstractText::DrawSymbol(glm::vec2& at, const text::Symbol* symbol) {
  return 0.F;
}

float AbstractText::DistanceTo(const glm::vec3& point) {
  return GetParent()->GetPosition().z + GetPositionOffset().z;  // sort by z
}

bool AbstractText::IsSortable() { return true; }

void AbstractText::UpdateState(const video::render::StateDef& state) {
  stateId_.State = state;
}

const video::render::StateIdentifier& AbstractText::StateId() const {
  return stateId_;
}

video::render::draw::Drawable* AbstractText::Drawable() { return this; }

int Glyph::AdvanceX() const {
  switch (Type) {
    case Type::Character:
      return Character->AdvanceX;
    case Type::Symbol:
      return Symbol->AdvanceX;
  }
}
int Glyph::SizeX() const {
  switch (Type) {
    case Type::Character:
      return Character->Size.x;
    case Type::Symbol:
      return Symbol->SizeX;
  }
}

void Word::Append(Glyph glyph) {
  Glyphs.push_back(glyph);
#ifdef DEBUG
  if (glyph.Type == Glyph::Type::Character) {
    Text += static_cast<char>(glyph.Character->Id);
  } else {
    Text += "¶";
  }

#endif
  Length += glyph.AdvanceX();
}

void Line::Append(const Word& word) {
  Words.push_back(word);
#ifdef DEBUG
  Text += word.Text;
#endif
  Length += word.Length;
}
void Line::Close() {
  if (!Words.empty() && !Words.back().Glyphs.empty()) {
    Length -= Words.back().Glyphs.back().AdvanceX() -
              Words.back().Glyphs.back().SizeX();
  }
}
}  // namespace soil::stage::scene::component::text
