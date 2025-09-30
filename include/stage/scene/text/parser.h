#ifndef SOIL_STAGE_SCENE_TEXT_PARSER_H
#define SOIL_STAGE_SCENE_TEXT_PARSER_H
#include <debug/vector>
#include <string>

#include "file/font.h"
#include "video/model/letter.h"

namespace soil::stage::scene::text {
/*
 struct Word {
     std::vector<const video::model::Letter*> Letters;
#ifdef DEBUG
     std::string Text;
#endif
     float Length;
     void Append(const video::model::Letter* letter, float size);
 };

 struct Line {
     std::vector<Word> Words;
#ifdef DEBUG
     std::string Text;
#endif
     float Length;
     void Append(const Word& word);
 };

 class Parser {
 public:
     ~Parser() = default;
     Parser(const Parser& other) = delete;
     Parser(Parser&& other) noexcept = delete;
     Parser& operator=(const Parser& other) = delete;
     Parser& operator=(Parser&& other) noexcept = delete;

     static std::vector<Line> Parse(const std::string& text, file::Font* font,
float letterSize, float maxLineLength);

 private:
     Parser() = delete;
 };*/
}  // namespace soil::stage::scene::text

#endif
