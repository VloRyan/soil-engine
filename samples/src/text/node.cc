#include "node.h"

#include "stage/scene/scene.h"

namespace soil_samples::text {
    Node::Node(soil::file::Font* font, const std::string& text) :
        soil::stage::scene::Node(Type::Visual), font_(font), size_(glm::vec2(0.0F)), letterOutline_(0.5F, 0.2F),
        borderOutline_(0.5F, 0.1F), letterSize_(1.0F), maxSize_(0.F), color_(glm::vec4(1.0F)), borderColor_(0.F),
        visible_(true) {
        Node::SetText(text);
    }

    void Node::SetText(const std::string& text) {
        if (text == text_) {
            return;
        }
        text_ = text;
        SetDirty(DirtyImpact::Self);
    }

    void Node::UpdateDirty() {
        if (IsDirtyImpact(DirtyImpact::Self)) {
            updateText();
        }
        soil::stage::scene::Node::UpdateDirty();
    }

    void Node::Word::Append(const soil::video::model::Letter* letter, const float size) {
        Letters.push_back(letter);
        Text += static_cast<char>(letter->Id);
        Length += letter->XAdvance * size;
    }

    void Node::Line::Append(const Word& word) {
        Words.push_back(word);
        Text += word.Text;
        Length += word.Length;
    }

    void Node::updateText() {
        const std::string text = GetText();
        std::vector<Line> lines;
        auto currentLine = Line();
        auto currentWord = Word();
        glm::vec2 cursorPosition(0);
        const auto& spaceLetter = font_->Characters.at(' ');
        const float maxLineLength = maxSize_.x * GetLetterSize();
        for (const auto c : text) {
            bool newWord = false;
            bool newLine = false;
            if (c == spaceLetter.Id) {
                newWord = true;
                if (maxLineLength > 0 && currentLine.Length > 0 &&
                    cursorPosition.x + currentWord.Length > maxLineLength) {
                    newLine = true;
                } else {
                    // currentWord.Append(spaceLetter, GetLetterSize());
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
                    const auto advance = letter->XAdvance * GetLetterSize();
                    if (maxLineLength > 0 && cursorPosition.x + advance > maxLineLength) {
                        break;
                    }
                    cursorPosition.x += advance;
                }
                currentLine.Append(currentWord);
                cursorPosition.x += spaceLetter.XAdvance * GetLetterSize();
                currentWord = Word();
            } else {
                const auto& letter = font_->Characters.at(c);
                // currentWord.Append(letter, GetLetterSize());
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
        const auto effectiveLineHeight = static_cast<float>(getFont()->LineHeight) * GetLetterSize();
        size_ = glm::vec2(0.F, effectiveLineHeight * static_cast<float>(lines.size()));
        for (auto& line : lines) {
            if (line.Length > size_.x) {
                size_.x = line.Length;
            }
        }
        cursorPosition.y = size_.y * 0.5F;
        auto letterIndex = 0;
        for (auto& line : lines) {
            cursorPosition.x = size_.x * -0.5F;
            cursorPosition.y -= effectiveLineHeight;
            for (auto& word : line.Words) {
                for (auto& letter : word.Letters) {
                    const auto advance = letter->XAdvance * GetLetterSize();
                    if (letterIndex == letterInstances_.size()) {
                        createInstance(letter, cursorPosition);
                    } else {
                        const auto letterInstance = letterInstances_[letterIndex];
                        letterInstance->SetLetter(letter, cursorPosition, static_cast<float>(getFont()->LineHeight),
                                                  GetLetterSize());
                        letterInstance->SetColor(GetColor());
                        letterInstance->SetBorderColor(GetBorderColor());
                        letterInstance->SetOutline(GetLetterOutline());
                        letterInstance->SetBorderOutline(GetBorderOutline());
                        letterInstance->UpdateTransform(GetWorldTransform());
                    }
                    letterIndex++;
                    cursorPosition.x += advance;
                }
            }
        }
        if (letterIndex < letterInstances_.size()) {
            for (auto i = letterIndex; i < letterInstances_.size(); ++i) {
                RemoveComponent(letterInstances_[i]);
            }
            letterInstances_.resize(letterIndex);
        }
    }

    void Node::createInstance(const soil::video::model::Letter* letter, const glm::vec2 offset) {
        const auto letterInstance = AddComponent(
            new LetterComponent(false, letter, static_cast<float>(getFont()->LineHeight), offset, GetLetterSize()));
        letterInstance->SetColor(GetColor());
        letterInstance->SetBorderColor(GetBorderColor());
        letterInstance->SetOutline(GetLetterOutline());
        letterInstance->SetBorderOutline(GetBorderOutline());
        letterInstances_.push_back(letterInstance);
    }

    glm::vec2 Node::GetSize() const {
        return size_;
    }

    void Node::SetBorderOutline(const glm::vec2& borderOutline) {
        if (borderOutline == borderOutline_) {
            return;
        }
        for (auto* letter : letterInstances_) {
            letter->SetBorderOutline(borderOutline);
        }
        borderOutline_ = borderOutline;
    }

    glm::vec2 Node::GetBorderOutline() const {
        return borderOutline_;
    }

    glm::vec2 Node::GetLetterOutline() const {
        return letterOutline_;
    }

    void Node::SetLetterOutline(const glm::vec2& charOutline) {
        if (charOutline == letterOutline_) {
            return;
        }
        for (auto* letter : letterInstances_) {
            letter->SetOutline(charOutline);
        }
        letterOutline_ = charOutline;
    }

    bool Node::IsVisible() const {
        return visible_;
    }

    glm::vec2 Node::GetMaxSize() const {
        return maxSize_;
    }

    void Node::SetMaxSize(const glm::vec2& max_size) {
        if (maxSize_ == max_size) {
            return;
        }
        maxSize_ = max_size;
        SetDirty(DirtyImpact::Self);
    }

    glm::vec4 Node::GetColor() const {
        return color_;
    }

    void Node::SetColor(const glm::vec4 color) {
        if (color == color_) {
            return;
        }
        color_ = color;
        for (auto* letter : letterInstances_) {
            letter->SetColor(color);
        }
    }

    glm::vec3 Node::GetBorderColor() const {
        return borderColor_;
    }

    void Node::SetBorderColor(const glm::vec3 color) {
        if (color == borderColor_) {
            return;
        }
        borderColor_ = color;
        for (auto* letter : letterInstances_) {
            letter->SetBorderColor(color);
        }
    }

    float Node::GetLetterSize() const {
        return letterSize_;
    }

    std::string Node::GetText() const {
        return text_;
    }

    void Node::SetLetterSize(const float letterSize) {
        if (letterSize == letterSize_) {
            return;
        }
        letterSize_ = letterSize;
        SetDirty(DirtyImpact::Self);
    }

    soil::file::Font* Node::getFont() const {
        return font_;
    }

    void Node::SetVisible(const bool visible) {
        if (visible == visible_) {
            return;
        }
        visible_ = visible;
        for (auto* letter : letterInstances_) {
            letter->SetVisible(visible);
        }
    }
} // namespace soil_samples::text
