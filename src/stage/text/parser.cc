#include "stage/text/parser.h"

#include "util/strings.h"

namespace soil::stage::text {
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

  auto cursor = Cursor();

  for (; cursor.Position < text.size(); cursor.Position++) {
    auto c = text.at(cursor.Position);
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
    auto markerStartType = getMarkerStartType(c);
    if (markerStartType != Marker::Types::None) {
      auto marker = parseMarker(text, markerStartType, cursor.Position);
      switch (marker.Type) {
        case Marker::Types::Symbol: {
          auto symbolItr = symbolMap.find(marker.SymbolName);
          if (symbolItr != symbolMap.end()) {
            currentWord.Append(Glyph{
                .Type = Glyph::Type::Symbol,
                .Symbol = &symbolItr->second,
            });
            cursor.Position = marker.End;
            continue;
          }
          break;
        }
        case Marker::Types::Attribute: {
          for (auto& attrib : marker.Attributes) {
            if (attrib.first == "color") {
              if (attrib.second.empty()) {
                cursor.Color.reset();
              } else {
                if (!cursor.Color.has_value()) {
                  cursor.Color = glm::vec4(1.F);
                }
                parseColor(attrib.second, cursor.Color.value());
              }
            }
          }
          cursor.Position = marker.End;
          continue;
        }
        default:;
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
          .Color = cursor.Color,
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
Parser::Marker::Types Parser::getMarkerStartType(char c) {
  switch (c) {
    case ':':
      return Marker::Types::Symbol;
    case '{':
      return Marker::Types::Attribute;
    default:
      return Marker::Types::None;
  }
}
Parser::Marker Parser::parseMarker(const std::string& text, Marker::Types type,
                                   int at) {
  auto marker = Parser::Marker{
      .Type = type,
      .Start = at,
      .End = -1,
  };
  switch (type) {
    case Marker::Types::Symbol:
      marker.End = static_cast<int>(text.find(':', at + 1));
      if (marker.End != -1) {
        marker.SymbolName =
            text.substr(marker.Start + 1, marker.End - marker.Start - 1);
      }
      break;
    case Marker::Types::Attribute: {
      marker.End = static_cast<int>(text.find('}', at + 1));
      if (marker.End != -1) {
        auto attribNameEnd = text.find('=', at + 1);
        std::string attribName;
        std::string attribValue;
        if (attribNameEnd == -1 || attribNameEnd > marker.End) {
          attribName =
              text.substr(marker.Start + 1, marker.End - marker.Start - 1);
        } else {
          attribName =
              text.substr(marker.Start + 1, attribNameEnd - marker.Start - 1);
          attribValue =
              text.substr(attribNameEnd + 1, marker.End - attribNameEnd - 1);
        }
        util::Strings::trim(attribName);
        util::Strings::trim(attribValue);
        marker.Attributes.insert_or_assign(attribName, attribValue);
      }
      break;
    }
    default:;
  }
  if (marker.End == -1) {
    marker.Type = Marker::Types::None;
  }
  return marker;
}

void Parser::parseColor(const std::string& text, glm::vec4& color) {
  switch (text.length()) {
    case 9:  // #rrggbbaa:
      color.a = static_cast<float>(std::stoul(text.substr(7, 2), nullptr, 16)) /
                255.F;
      [[fallthrough]];
    case 7:  // #rrggbb:
      color.r = static_cast<float>(std::stoul(text.substr(1, 2), nullptr, 16)) /
                255.F;
      color.g = static_cast<float>(std::stoul(text.substr(3, 2), nullptr, 16)) /
                255.F;
      color.b = static_cast<float>(std::stoul(text.substr(5, 2), nullptr, 16)) /
                255.F;
      break;
    default:
      throw std::runtime_error("invalid color format: " + text);
  }
}
}  // namespace soil::stage::text
