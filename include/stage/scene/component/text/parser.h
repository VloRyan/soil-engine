#ifndef SOIL_STAGE_SCENE_COMPONENT_TEXT_PARSER_H
#define SOIL_STAGE_SCENE_COMPONENT_TEXT_PARSER_H
#include <debug/vector>
#include <string>

#include "abstract_text.h"
#include "file/font.h"

namespace soil::stage::scene::component::text {
class Parser {
 public:
  Parser() = delete;

  ~Parser() = default;

  Parser(const Parser& other) = delete;

  Parser(Parser&& other) noexcept = delete;

  Parser& operator=(const Parser& other) = delete;

  Parser& operator=(Parser&& other) noexcept = delete;

  static std::vector<Line> Parse(const std::string& text,
                                 const file::Font* font, int maxLineLength);
};
}  // namespace soil::stage::scene::component::text

#endif
