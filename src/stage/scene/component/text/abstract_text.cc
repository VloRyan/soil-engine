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
      maxSize_(0.F),
      color_(glm::vec4(1.0F)),
      borderColor_(0.F) {
  stateId_ = {
      .Shader = data_->Shader,
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

/*
float AbstractText::DistanceTo(const glm::vec3& point) {
  return glm::distance(GetParent()->GetPosition().z + positionOffset_.z,
                       point.z);
}*/

const std::vector<Line>& AbstractText::GetLines() const { return lines_; }

void AbstractText::SetPositionOffset(const glm::vec3& positionOffset) {
  positionOffset_ = positionOffset;
  SignalChanged();
}

glm::vec3 AbstractText::GetPositionOffset() const { return positionOffset_; }

void AbstractText::updateText() {
  lines_ = Parser::Parse(GetText(), data_->Font, static_cast<int>(maxSize_.x));
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

glm::vec2 AbstractText::GetMaxSize() const { return maxSize_; }

void AbstractText::SetMaxSize(const glm::vec2& max_size) {
  if (maxSize_ == max_size) {
    return;
  }
  maxSize_ = max_size;
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

void AbstractText::Bind(video::render::State& state) {
  state.Apply(stateId_.State);
  state.SetShader(stateId_.Shader);
  stateId_.Shader->Prepare(state);
  state.BindVao(stateId_.Vao);
}

void AbstractText::Draw() {
  glm::vec2 cursorPosition;
  const auto effectiveLineHeight =
      GetSize().y / static_cast<float>(GetLines().size());
  cursorPosition.y =
      effectiveLineHeight * static_cast<float>(GetLines().size() + 1) * 0.5F;

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

        const auto parentPos = GetParent()->GetPosition();
        const auto worldPos = glm::vec3(localPosition.x + parentPos.x,
                                        localPosition.y + parentPos.y,
                                        parentPos.z + GetPositionOffset().z);
        SetupCharacter(character, worldPos);
        const auto* ebo = stateId_.Vao->GetEbo();
        soil::video::shader::Program::DrawElements(
            static_cast<uint>(video::render::DrawMode::TriangleStrip),
            ebo->GetIndexCount(), ebo->GetIndexType());

        const auto advance =
            static_cast<float>(character.XAdvance) * GetCharacterSize();
        cursorPosition.x += advance;
      }
    }
    cursorPosition.y -= effectiveLineHeight;
  }
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
}  // namespace soil::stage::scene::component::text
