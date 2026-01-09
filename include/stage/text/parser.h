#ifndef SOIL_STAGE_TEXT_PARSER_H
#define SOIL_STAGE_TEXT_PARSER_H
#include <string>
#include <vector>

#include "file/font.h"
#include "types.hpp"

namespace soil::stage::text {
class Parser {
 public:
  Parser() = delete;
  ~Parser() = default;
  Parser(const Parser& other) = delete;
  Parser(Parser&& other) noexcept = delete;
  Parser& operator=(const Parser& other) = delete;
  Parser& operator=(Parser&& other) noexcept = delete;
  static std::vector<Line> Parse(
      const std::string& text,
      const std::unordered_map<int, file::Font::Character>& characterMap,
      int maxLineLength = -1);
  static std::vector<Line> Parse(
      const std::string& text,
      const std::unordered_map<int, file::Font::Character>& characterMap,
      const std::unordered_map<std::string, Symbol>& symbolMap,
      int maxLineLength = -1);

 private:
  struct Cursor {
    int Position{0};
    std::optional<glm::vec4> Color;
  };

  struct Marker {
    enum class Types : std::uint8_t {
      None = 0,
      Symbol,
      Attribute,
    };
    Types Type{Types::None};
    int Start{-1};
    int End{-1};
    std::string SymbolName;
    std::unordered_map<std::string, std::string> Attributes;
  };
  static Marker::Types getMarkerStartType(char c);
  static Marker parseMarker(const std::string& text, Marker::Types type,
                            int at);

  static void parseColor(const std::string& text, glm::vec4& color);
};
}  // namespace soil::stage::text

#endif
