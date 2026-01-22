#ifndef SOIL_STAGE_TEXT_TYPES_HPP
#define SOIL_STAGE_TEXT_TYPES_HPP
#include <unordered_map>

#include "file/font.h"
#include "file/sprite_sheet.h"
#include "video/model/letter.h"
#include "video/texture/texture.h"
namespace soil::stage::text {

struct Symbol {
  std::string Name{-1};
  const video::texture::Texture* Texture{nullptr};
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

  [[nodiscard]] int AdvanceX() const {
    switch (Type) {
      case Type::Character:return Character->AdvanceX;
      case Type::Symbol:return Symbol->AdvanceX;
    }
  }

  [[nodiscard]] int SizeX() const {
    switch (Type) {
      case Type::Character:return Character->Size.x;
      case Type::Symbol:return Symbol->SizeX;
    }
  }

  Glyph::Type Type{Glyph::Type::Character};
  const file::Font::Character* Character{nullptr};
  const text::Symbol* Symbol{nullptr};
  const std::optional<glm::vec4> Color{};

  bool operator==(const Glyph& rhs) const {
    return Type == rhs.Type && Character == rhs.Character &&
        Symbol == rhs.Symbol;
  }

  bool operator!=(const Glyph& rhs) const { return !(rhs == *this); }
};

struct Word {
  std::vector<Glyph> Glyphs{};
#ifdef DEBUG
  std::string Text{};
#endif
  int Length{0};

  void Append(Glyph glyph) {
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

  bool operator==(const Word& rhs) const {
    return Glyphs == rhs.Glyphs && Text == rhs.Text && Length == rhs.Length;
  }

  bool operator!=(const Word& rhs) const { return !(rhs == *this); }
};

struct Line {
  std::vector<Word> Words{};
#ifdef DEBUG
  std::string Text{};
#endif
  int Length{0};

  void Append(const Word& word) {
    Words.push_back(word);
#ifdef DEBUG
    Text += word.Text;
#endif
    Length += word.Length;
  }

  void Close() {
    if (!Words.empty() && !Words.back().Glyphs.empty()) {
      Length -= Words.back().Glyphs.back().AdvanceX() -
          Words.back().Glyphs.back().SizeX();
    }
  }
};
}  // namespace soil::stage::text
#endif
