#include "stage/scene/component/text/parser.h"

namespace soil::stage::scene::component::text {
    std::vector<Line> Parser::Parse(const std::string& text, const file::Font* font, const int maxLineLength) {
        std::vector<Line> lines;
        auto currentLine = Line();
        auto currentWord = Word();
        glm::ivec2 cursorPosition(0);
        const auto& spaceCharacter = font->Characters.at(' ');
        for (const auto c : text) {
            bool newWord = false;
            bool newLine = false;
            if (c == spaceCharacter.Id) {
                newWord = true;
                if (maxLineLength > 0 && currentLine.Length > 0 &&
                    cursorPosition.x + currentWord.Length > maxLineLength) {
                    newLine = true;
                } else {
                    currentWord.Append(&spaceCharacter);
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
                for (const auto letter : currentWord.Characters) {
                    const auto advance = letter->XAdvance;
                    if (maxLineLength > 0 && cursorPosition.x + advance > maxLineLength) {
                        break;
                    }
                    cursorPosition.x += advance;
                }
                /*if (!currentWord.Characters.empty()) {
                     currentWord.Length -= currentWord.Characters.back()->XAdvance -
                         currentWord.Characters.back()->Size.x - font->Padding[1];
                }*/
                currentLine.Append(currentWord);
                cursorPosition.x += spaceCharacter.XAdvance;
                currentWord = Word();
            } else {
                auto& character = font->Characters.at(c);
                currentWord.Append(&character);
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
            if (!currentWord.Characters.empty()) {
                currentWord.Length -=
                    currentWord.Characters.back()->XAdvance - currentWord.Characters.back()->Size.x - font->Padding[1];
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

    void Word::Append(const file::Font::Character* character) {
        Characters.push_back(character);
#ifdef DEBUG
        Text += static_cast<char>(character->Id);
#endif
        Length += character->XAdvance;
    }

    void Line::Append(const Word& word) {
        Words.push_back(word);
#ifdef DEBUG
        Text += word.Text;
#endif
        Length += word.Length;
    }
} // namespace soil::stage::scene::component::text
