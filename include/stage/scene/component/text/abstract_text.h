#ifndef SOIL_STAGE_SCENE_COMPONENT_TEXT_ABSTRACT_TEXT_H
#define SOIL_STAGE_SCENE_COMPONENT_TEXT_ABSTRACT_TEXT_H
#include <unordered_map>

#include "file/font.h"
#include "stage/scene/component/render/mesh_component.h"
#include "stage/scene/node.h"
#include "video/manager.h"
#include "video/model/letter.h"
#include "video/texture/texture.h"

namespace soil::stage::scene::component::text {
struct Symbol {
  std::string Name{-1};
  video::texture::Texture* Texture{nullptr};
  int TileIndex{0};
  int AdvanceX{0};
  int SizeX{0};

  bool operator==(const Symbol& rhs) const {
    return Name == rhs.Name && Texture == rhs.Texture &&
           TileIndex == rhs.TileIndex;
  }
  bool operator!=(const Symbol& rhs) const { return !(rhs == *this); }
};

struct Glyph {
  enum class Type : std::uint8_t {
    Character = 0,
    Symbol,
  };
  [[nodiscard]] int AdvanceX() const;
  [[nodiscard]] int SizeX() const;
  Glyph::Type Type{Glyph::Type::Character};
  const file::Font::Character* Character{nullptr};
  const text::Symbol* Symbol{nullptr};
  bool operator==(const Glyph& rhs) const {
    return Type == rhs.Type && Character == rhs.Character &&
           Symbol == rhs.Symbol;
  }
  bool operator!=(const Glyph& rhs) const { return !(rhs == *this); }
};

struct Word {
  std::vector<Glyph> Glyphs{};
  std::vector<Symbol> Symbols{};
#ifdef DEBUG
  std::string Text{};
#endif
  int Length{0};

  void Append(Glyph glyph);
  bool operator==(const Word& rhs) const {
    return Glyphs == rhs.Glyphs && Symbols == rhs.Symbols && Text == rhs.Text &&
           Length == rhs.Length;
  }
  bool operator!=(const Word& rhs) const { return !(rhs == *this); }
};

struct Line {
  std::vector<Word> Words{};
#ifdef DEBUG
  std::string Text{};
#endif
  int Length{0};
  void Append(const Word& word);
  void Close();
};

class AbstractText : public DrawableComponent,
                     public video::render::draw::Drawable {
 public:
  struct PrefabData {
    video::vertex::Vao* QuadVao{nullptr};
    video::shader::Program* CharacterShader{nullptr};
    video::shader::Program* SymbolShader{nullptr};
    const file::Font* Font{nullptr};
    video::texture::Texture* FontTexture{nullptr};
    std::unordered_map<std::string, Symbol> SymbolMap{};
  };

  explicit AbstractText(const std::string& prefab,
                        const std::string& text = "");

  ~AbstractText() override = default;

  static void InitPrefab(const std::string& name, const PrefabData& data);

  static bool HasPrefab(const std::string& name);

  static void RemovePrefab(const std::string& name);

  [[nodiscard]] virtual std::string GetText() const;

  virtual void SetText(const std::string& text);

  [[nodiscard]] glm::vec2 GetSize() const;

  virtual void SetBorderOutline(const glm::vec2& borderOutline);

  [[nodiscard]] glm::vec2 GetBorderOutline() const;

  [[nodiscard]] glm::vec2 GetCharacterOutline() const;

  virtual void SetCharacterOutline(const glm::vec2& characterOutline);

  virtual void SetCharacterSize(float size);

  [[nodiscard]] virtual float GetCharacterSize() const;

  void SetColor(glm::vec4 color);

  [[nodiscard]] glm::vec4 GetColor() const;

  void SetBorderColor(glm::vec3 color);

  [[nodiscard]] glm::vec3 GetBorderColor() const;

  [[nodiscard]] const file::Font* GetFont() const;

  [[nodiscard]] virtual int GetMaxLineLength() const;
  virtual void SetMaxLineLength(int maxLineLength);

  [[nodiscard]] virtual byte GetTextureSlot() const;

  virtual void SetPositionOffset(const glm::vec3& positionOffset);
  [[nodiscard]] virtual glm::vec3 GetPositionOffset() const;

  const std::vector<Line>& GetLines() const;

  [[nodiscard]] const video::render::StateIdentifier& StateId() const override;
  void Draw(video::render::State& state) override;
  float DistanceTo(const glm::vec3& point) override;
  bool IsSortable() override;
  class Drawable* Drawable() override;

  void UpdateState(const video::render::StateDef& state);

  virtual void SetupCharacter(const file::Font::Character& character,
                              const glm::vec3& worldPos,
                              video::shader::Program* shader) = 0;

  virtual void SetupSymbol(const text::Symbol* symbol,
                           const glm::vec3& worldPos,
                           video::shader::Program* shader) = 0;

  virtual void SetupText(video::render::State& state) {};

 protected:
  struct SymbolPosition {
    const text::Symbol* Symbol{nullptr};
    glm::vec3 Position{};
  };
  float DrawCharacter(const glm::vec3& at,
                      const file::Font::Character& character);
  float DrawSymbol(glm::vec2& cursorPos, const text::Symbol* symbol);
  const PrefabData& Data();

  PrefabData* data_;

 private:
  static std::unordered_map<std::string, PrefabData> PREFABS;

  void updateText();

  glm::vec2 size_;

  glm::vec3 positionOffset_;

  glm::vec2 characterOutline_;
  glm::vec2 borderOutline_;

  std::string text_;
  float characterSize_;
  int maxLineLength_;
  glm::vec4 color_;
  glm::vec3 borderColor_;

  std::vector<Line> lines_;
  video::render::StateIdentifier stateId_;
};
}  // namespace soil::stage::scene::component::text

#endif
