#include "stage/scene/component/text/abstract_text.h"

#include "stage/scene/component/text/parser.h"

namespace soil::stage::scene::component::text {
std::unordered_map<std::string, AbstractText::PrefabData>
    AbstractText::PREFABS = {};

AbstractText::AbstractText(const std::string& prefab, const std::string& text)
    : MeshComponent(*PREFABS[prefab].MeshData, PREFABS[prefab].Shader, false),
      data_(&PREFABS[prefab]),
      size_(glm::vec2(0.0F)),
      positionOffset_(0.F),
      characterOutline_(0.5F, 0.2F),
      borderOutline_(0.5F, 0.1F),
      characterSize_(1.0F),
      maxSize_(0.F),
      color_(glm::vec4(1.0F)),
      borderColor_(0.F) {
  AbstractText::SetText(text);
  mesh_->SetRenderFunc(
      [this](soil::video::render::State& state, int count) { Render(state); });
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

float AbstractText::DistanceTo(const glm::vec3& point) {
  return glm::distance(GetParent()->GetPosition().z + positionOffset_.z,
                       point.z);
}

void AbstractText::Update() { MeshComponent::Update(); }

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
}  // namespace soil::stage::scene::component::text
