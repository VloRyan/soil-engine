#include "stage/scene/text/parser.h"

namespace soil::stage::scene::text { /*
     std::vector<Line> Parser::Parse(const std::string& text, file::Font* font, const float letterSize,
                                     const float maxLineLength) {
         std::vector<Line> lines;
         auto currentLine = Line();
         auto currentWord = Word();
         glm::vec2 cursorPosition(0);
         const video::model::Letter* spaceLetter = font->GetLetter(' ');
         for (const auto c : text) {
             bool newWord = false;
             bool newLine = false;
             if (c == spaceLetter->Id) {
                 newWord = true;
                 if (maxLineLength > 0 && currentLine.Length > 0 &&
                     cursorPosition.x + currentWord.Length > maxLineLength) {
                     newLine = true;
                 } else {
                     currentWord.Append(spaceLetter, letterSize);
                 }
             }
             if (c == '\n') {
                 newWord = true;
             }
             if (newLine) {
                 cursorPosition.x = 0;
                 lines.push_back(currentLine);
                 currentLine = Line();
             }
             if (newWord) {
                 for (const auto letter : currentWord.Letters) {
                     const auto advance = letter->XAdvance * letterSize;
                     if (maxLineLength > 0 && cursorPosition.x + advance > maxLineLength) {
                         break;
                     }
                     cursorPosition.x += advance;
                 }
                 currentLine.Append(currentWord);
                 cursorPosition.x += spaceLetter->XAdvance * letterSize;
                 currentWord = Word();
             } else {
                 const video::model::Letter* letter = font->GetLetter(static_cast<std::uint32_t>(c));
                 if (letter == nullptr) {
                     continue;
                 }
                 currentWord.Append(letter, letterSize);
             }
             if (c == '\n') {
                 cursorPosition.x = 0;
                 lines.push_back(currentLine);
                 currentLine = Line();
             }
         }
         if (currentWord.Length > 0) {
             if (maxLineLength > 0 && currentLine.Length > 0 && cursorPosition.x + currentWord.Length > maxLineLength) {
                 cursorPosition.x = 0;
                 lines.push_back(currentLine);
                 currentLine = Line();
             }
             currentLine.Append(currentWord);
             lines.push_back(currentLine);
             currentLine = Line();
         }
         if (currentLine.Length > 0) {
             lines.push_back(currentLine);
         }

         return lines;
     }

     void Word::Append(const video::model::Letter* letter, const float size) {
         Letters.push_back(letter);
 #ifdef DEBUG
         Text += static_cast<char>(letter->Id);
 #endif
         Length += letter->XAdvance * size;
     }

     void Line::Append(const Word& word) {
         Words.push_back(word);
 #ifdef DEBUG
         Text += word.Text;
 #endif
         Length += word.Length;
     }*/
} // namespace soil::stage::scene::text
