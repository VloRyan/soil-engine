#ifndef SOIL_FILE_FONT_H
#define SOIL_FILE_FONT_H

#include <debug/vector>
#include <string>
#include <unordered_map>

#include "video/model/letter.h"

namespace soil::file {
struct Font {
  struct Character {
    int Id{-1};
    glm::ivec2 TextureCoord{0, 0};
    glm::ivec2 Offset{0, 0};
    glm::ivec2 Size{0, 0};
    int AdvanceX{0};

    [[nodiscard]] bool IsPrintable() const;

    friend bool operator==(const Character& lhs, const Character& rhs) {
      return lhs.Id == rhs.Id && lhs.TextureCoord == rhs.TextureCoord &&
             lhs.Offset == rhs.Offset && lhs.Size == rhs.Size &&
             lhs.AdvanceX == rhs.AdvanceX;
    }

    friend bool operator!=(const Character& lhs, const Character& rhs) {
      return !(lhs == rhs);
    }
  };

  struct Kerning {
    int First;
    int Second;
    int Amount;

    friend bool operator==(const Kerning& lhs, const Kerning& rhs) {
      return lhs.First == rhs.First && lhs.Second == rhs.Second &&
             lhs.Amount == rhs.Amount;
    }

    friend bool operator!=(const Kerning& lhs, const Kerning& rhs) {
      return !(lhs == rhs);
    }
  };

  std::string FileName{};
  int LineHeight{0};
  int Base{0};
  glm::ivec4 Padding{0, 0, 0, 0};
  std::unordered_map<int, Character> Characters{};
  std::vector<Kerning> Kernings{};
  std::string TextureFileName{};
  glm::ivec2 ImageSize{};

  static Font* Load(const std::string& filePath);

 private:
  enum class LineType : std::int8_t {
    Info = 0,
    Common,
    Page,
    Chars,
    Char,
    Kernings,
    Kerning,
    Unknown
  };

  static LineType parseLineValues(
      const std::string& line,
      std::unordered_map<std::string, std::string>& values);

  static int getIntValue(const std::string& key,
                         std::unordered_map<std::string, std::string>& values);

  static std::string getValue(
      const std::string& key,
      std::unordered_map<std::string, std::string>& values);

  static void parseInfos(Font* fontFile,
                         std::unordered_map<std::string, std::string>& values);

  static void parseCommon(Font* fontFile,
                          std::unordered_map<std::string, std::string>& values);

  static void parsePage(Font* fontFile,
                        std::unordered_map<std::string, std::string>& values);

  static void parseChar(Font* fontFile,
                        std::unordered_map<std::string, std::string>& values);

  static void parseKerning(
      Font* fontFile, std::unordered_map<std::string, std::string>& values);
};
}  // namespace soil::file

#endif
