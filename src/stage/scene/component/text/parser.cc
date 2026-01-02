#include "stage/scene/component/text/parser.h"

namespace soil::stage::scene::component::text {
std::vector<Line> Parser::Parse(
    const std::string& text,
    const std::unordered_map<int, file::Font::Character>& characterMap,
    const int maxLineLength) {
  return Parse(text, characterMap, {}, maxLineLength);
}

std::vector<Line> Parser::Parse(
    const std::string& text,
    const std::unordered_map<int, file::Font::Character>& characterMap,
    const std::unordered_map<std::string, Symbol>& symbolMap,
    int maxLineLength) {
  std::vector<Line> lines;
  auto currentLine = Line();
  auto currentWord = Word();
  glm::ivec2 cursorPosition(0);
  const auto& spaceGlyph = Glyph{
      .Type = Glyph::Type::Character,
      .Character = &characterMap.at(' '),
  };
  for (auto i = 0; i < text.size(); i++) {
    auto c = text.at(i);
    bool newWord = false;
    bool newLine = false;
    if (c == spaceGlyph.Character->Id) {
      newWord = true;
      if (maxLineLength > 0 && currentLine.Length > 0 &&
          cursorPosition.x + currentWord.Length > maxLineLength) {
        newLine = true;
      } else {
        currentWord.Append(spaceGlyph);
      }
    }
    if (c == '\n') {
      newWord = true;
    }
    if (c == ':') {
      int markerEnd = -1;
      for (auto j = i + 1; j < text.size(); j++) {
        auto m = text.at(j);
        if (m == ':') {
          markerEnd = j;
          break;
        }
        if (m == ' ') {
          break;
        }
      }
      if (markerEnd != -1) {
        auto name = text.substr(i + 1, markerEnd - i - 1);
        if (symbolMap.contains(name)) {
          currentWord.Append(Glyph{
              .Type = Glyph::Type::Symbol,
              .Symbol = &symbolMap.at(name),
          });
          i = markerEnd;
          continue;
        }
      }
    }
    if (newLine) {
      cursorPosition.x = 0;
      currentLine.Close();
      lines.push_back(currentLine);
      currentLine = Line();
    }
    if (newWord) {
      for (const auto& glyph : currentWord.Glyphs) {
        const auto advance = glyph.AdvanceX();
        if (maxLineLength > 0 && cursorPosition.x + advance > maxLineLength) {
          break;
        }
        cursorPosition.x += advance;
      }
      currentLine.Append(currentWord);
      cursorPosition.x += spaceGlyph.AdvanceX();
      currentWord = Word();
    } else {
      currentWord.Append(Glyph{
          .Type = Glyph::Type::Character,
          .Character = &characterMap.at(c),
      });
    }
    if (c == '\n') {
      cursorPosition.x = 0;
      currentLine.Close();
      lines.push_back(currentLine);
      currentLine = Line();
    }
  }
  if (currentWord.Length > 0) {
    if (maxLineLength > 0 && currentLine.Length > 0 &&
        cursorPosition.x + currentWord.Length > maxLineLength) {
      cursorPosition.x = 0;
      currentLine.Close();
      lines.push_back(currentLine);
      currentLine = Line();
    }
    currentLine.Append(currentWord);
    currentLine.Close();
    lines.push_back(currentLine);
    currentLine = Line();
  }
  if (currentLine.Length > 0) {
    lines.push_back(currentLine);
  }

  return lines;
}

}  // namespace soil::stage::scene::component::text
