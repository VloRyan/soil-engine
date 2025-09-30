#include "file/font.h"

#include <fstream>
#include <ranges>
#include <utility>
#include <vector>

#include "util/files.h"
#include "util/strings.h"

namespace soil::file {
Font* Font::Load(const std::string& filePath) {
  if (std::string extension = util::Files::GetExtension(filePath);
      extension != ".fnt") {
    throw std::invalid_argument("File must be .fnt file.");
  }
  if (!util::Files::Exists(filePath)) {
    throw std::runtime_error("File not found: " + filePath);
  }

  std::ifstream ifs;
  ifs.open(filePath, std::ios::in);
  if (ifs.fail()) {
    std::string message = "Failed to open file: " + filePath;
    throw std::runtime_error(message);
  }

  auto* fontFile = new Font{
      .FileName = filePath,
      .LineHeight = 0,
      .Base = 0,
      .Padding = glm::ivec4(0),
      .Characters = {},
      .Kernings = {},
      .TextureFileName = "",
      .ImageSize = glm::ivec2(0),
  };
  std::string line;
  std::unordered_map<std::string, std::string> values;
  auto charCount = 0;
  auto kerningCount = 0;
  while (std::getline(ifs, line)) {
    switch (parseLineValues(line, values)) {
      case LineType::Info:
        parseInfos(fontFile, values);
        break;
      case LineType::Common:
        parseCommon(fontFile, values);
        break;
      case LineType::Page:
        parsePage(fontFile, values);
        break;
      case LineType::Chars:
        charCount = getIntValue("count", values);
        break;
      case LineType::Char:
        parseChar(fontFile, values);
        break;
      case LineType::Kernings:
        kerningCount = getIntValue("count", values);
        break;
      case LineType::Kerning:
        parseKerning(fontFile, values);
        break;
      default:;  // ignore
    }
    values.clear();
  }
  ifs.close();
  if (charCount != fontFile->Characters.size()) {
    throw std::runtime_error("Char count mismatch");
  }
  if (kerningCount != fontFile->Kernings.size()) {
    throw std::runtime_error("Kerning count mismatch");
  }
  return fontFile;
}

Font::LineType Font::parseLineValues(
    const std::string& line,
    std::unordered_map<std::string, std::string>& values) {
  const std::vector<std::string> parts = util::Strings::split(line, " ");
  auto lineType = LineType::Unknown;
  std::string key;
  std::string value;
  bool valueStartsWithDQuote = false;
  bool valueEndsWithDQuote = false;
  for (const auto& part : parts) {
    std::string token;
    if (part.empty()) {
      continue;
    }
    if (const std::string::size_type posOfEqual = part.find('=');
        posOfEqual != std::string::npos) {
      key = part.substr(0, posOfEqual);
      value = part.substr(posOfEqual + 1, token.length() - (posOfEqual + 1));
    } else if (valueStartsWithDQuote) {
      value += token;
    } else {
      if (values.empty()) {
        if (part == "info") {
          lineType = LineType::Info;
        } else if (part == "common") {
          lineType = LineType::Common;
        } else if (part == "page") {
          lineType = LineType::Page;
        } else if (part == "chars") {
          lineType = LineType::Chars;
        } else if (part == "char") {
          lineType = LineType::Char;
        } else if (part == "kernings") {
          lineType = LineType::Kernings;
        } else if (part == "kerning") {
          lineType = LineType::Kerning;
        }
        continue;
      } else {
        auto msg = std::string("Invalid state(Part: ") + part;
        msg += " Key: " + key;
        msg += " Value: " + value + ")";
        throw std::runtime_error(msg);
      }
    }
    valueStartsWithDQuote = value.starts_with('\"');            // starts with "
    valueEndsWithDQuote = value.at(value.length() - 1) == '"';  // ends with "
    if (const bool valueComplete =
            !valueStartsWithDQuote || valueEndsWithDQuote;
        valueComplete) {
      if (valueStartsWithDQuote) {
        value = value.substr(1, value.length() - 1);
      }
      if (valueEndsWithDQuote) {
        value = value.substr(0, value.length() - 1);
      }
      values.insert({key, value});
    }
  }
  return lineType;
}

std::string Font::getValue(
    const std::string& key,
    std::unordered_map<std::string, std::string>& values) {
  const auto itr = values.find(key);
  if (itr == values.end()) {
    return "";
  }
  return itr->second;
}

int Font::getIntValue(const std::string& key,
                      std::unordered_map<std::string, std::string>& values) {
  return std::stoi(getValue(key, values));
}

void Font::parseInfos(Font* fontFile,
                      std::unordered_map<std::string, std::string>& values) {
  const auto paddingStr = values["padding"];
  const auto parts = util::Strings::split(paddingStr, ",");
  if (parts.size() != 4) {
    throw std::runtime_error("Invalid padding size: " +
                             std::to_string(parts.size()));
  }
  for (auto i = 0; i < parts.size(); i++) {
    fontFile->Padding[i] = std::stoi(parts[i]);
  }
}

void Font::parseCommon(Font* fontFile,
                       std::unordered_map<std::string, std::string>& values) {
  fontFile->ImageSize =
      glm::ivec2(getIntValue("scaleW", values), getIntValue("scaleH", values));
  fontFile->LineHeight = getIntValue("lineHeight", values);
  fontFile->Base = getIntValue("base", values);
}

void Font::parsePage(Font* fontFile,
                     std::unordered_map<std::string, std::string>& values) {
  auto texFile = values["file"];
  if (util::Files::GetDirectory(texFile).empty()) {
    const std::string fontFileDir =
        util::Files::GetDirectory(fontFile->FileName);
    texFile = fontFileDir + texFile;
  }
  fontFile->TextureFileName = texFile;
}

void Font::parseChar(Font* fontFile,
                     std::unordered_map<std::string, std::string>& values) {
  auto id = getIntValue("id", values);
  auto character = Character{
      .Id = id,
      .TextureCoord =
          glm::ivec2(getIntValue("x", values), getIntValue("y", values)),
      .Offset = glm::ivec2(getIntValue("xoffset", values),
                           getIntValue("yoffset", values)),
      .Size = glm::ivec2(getIntValue("width", values),
                         getIntValue("height", values)),
      .XAdvance = getIntValue("xadvance", values),
  };
  fontFile->Characters.insert({id, character});
}

void Font::parseKerning(Font* fontFile,
                        std::unordered_map<std::string, std::string>& values) {
  fontFile->Kernings.emplace_back(Kerning{
      .First = getIntValue("first", values),
      .Second = getIntValue("second", values),
      .Amount = getIntValue("amount", values),
  });
}
}  // namespace soil::file
