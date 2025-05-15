#include "file/font.h"

#include <fstream>
#include <ranges>
#include <utility>
#include <vector>
#include "util/files.h"
#include "util/strings.h"

namespace soil::file {
    Font::Font(std::string fileName) :
        fileName_(std::move(fileName)), lineHeight_(0), base_(0), spaceWidth_(0), padding_(0), imageSize_(0) {}

    Font::~Font() {
        for (const auto *letter : characterMap_ | std::views::values) {
            delete letter;
        }
    }

    glm::vec4 Font::getPadding() const {
        return padding_;
    }

    float Font::getSpaceWidth() const {
        return spaceWidth_;
    }

    video::model::Letter *Font::getCharacter(const uint id) {
        if (const auto itr = characterMap_.find(id); itr != characterMap_.end()) {
            return itr->second;
        }
        return nullptr;
    }

    std::string Font::getTextureFile() const {
        return textureFile_;
    }

    float Font::getHorizontalPadding() {
        return padding_[1] + padding_[3];
    }

    float Font::getVerticalPadding() {
        return padding_[0] + padding_[2];
    }

    uint Font::getImageSize() const {
        return imageSize_;
    }

    std::string Font::getFileName() const {
        return fileName_;
    }

    float Font::getLineHeight() const {
        return lineHeight_;
    }

    float Font::getBase() const {
        return base_;
    }

    Font *Font::Load(std::string fontFilePath) {
        bool isFontFile = false;
        if (std::string extension = util::Files::GetExtension(fontFilePath); extension.empty()) {
            fontFilePath += ".fnt";
            if (util::Files::Exists(fontFilePath)) {
                isFontFile = true;
            }
        } else if (extension == "fnt") {
            isFontFile = true;
        }
        if (!isFontFile) {
            throw std::runtime_error("No .fnt file found for expression '" + fontFilePath + "'.");
        }
        std::ifstream ifs;
        ifs.open(fontFilePath, std::ios::in);
        if (ifs.fail()) {
            std::string message = "Failed to open file: " + fontFilePath;
            throw std::runtime_error(message);
        }

        auto *fontFile = new Font(fontFilePath);
        std::string line;
        std::unordered_map<std::string, std::string> mappedValues;
        while (std::getline(ifs, line)) {
            mapValues(line, mappedValues);
            std::string lineType = getValue("LINE_TYPE", mappedValues);
            if (lineType == "info") {
                loadInfos(fontFile, mappedValues);
            }
            if (lineType == "common") {
                loadCommon(fontFile, mappedValues);
            }
            if (lineType == "page") {
                loadPage(fontFile, mappedValues);
            }
            if (lineType == "chars") {
                loadChars(fontFile, mappedValues);
            }
            if (lineType == "char") {
                loadChar(fontFile, mappedValues);
            }
        }
        ifs.close();
        float factor = 12.0F / (fontFile->getLineHeight() - fontFile->getBase());
        fontFile->lineHeight_ = fontFile->lineHeight_ * factor;
        return fontFile;
    }

    void Font::mapValues(std::string line, std::unordered_map<std::string, std::string> &map) {
        const std::vector<std::string> parts = util::Strings::split(std::move(line), " ");
        std::string key;
        std::string value;
        bool valueStartsWithDQuote = false;
        bool valueEndsWithDQuote = false;
        map.clear();
        for (const std::string &part : parts) {
            std::string token;
            if (part.empty()) {
                continue;
            }
            if (const std::string::size_type posOfEqual = part.find('='); posOfEqual != std::string::npos) {
                key = part.substr(0, posOfEqual);
                value = part.substr(posOfEqual + 1, token.length() - (posOfEqual + 1));
            } else if (valueStartsWithDQuote) {
                value += token;
            } else {
                if (map.empty()) {
                    key = "LINE_TYPE";
                    value = part;
                } else {
                    auto msg = std::string("Invalid state(Part: ") + part;
                    msg += " Key: " + key;
                    msg += " Value: " + value + ")";
                    throw std::runtime_error(msg);
                }
            }
            valueStartsWithDQuote = value.starts_with('\"'); // starts with "
            valueEndsWithDQuote = value.at(value.length() - 1) == '"'; // ends with "
            if (const bool valueComplete = !valueStartsWithDQuote || valueEndsWithDQuote; valueComplete) {
                if (valueStartsWithDQuote) {
                    value = value.substr(1, value.length() - 1);
                }
                if (valueEndsWithDQuote) {
                    value = value.substr(0, value.length() - 1);
                }
                std::unordered_map<std::string, std::string>::value_type pair(key, value);
                map.insert(pair);
            }
        }
    }

    std::string Font::getValue(const std::string &key, std::unordered_map<std::string, std::string> &map) {
        const auto itr = map.find(key);
        if (itr == map.end()) {
            return "";
        }
        return itr->second;
    }

    int Font::getIntValue(const std::string &key, std::unordered_map<std::string, std::string> &map) {
        return std::stoi(getValue(key, map));
    }

    void Font::loadInfos(Font *fontFile, std::unordered_map<std::string, std::string> &map) {
        const std::string paddingStr = getValue("padding", map);
        const std::vector<std::string> parts = util::Strings::split(paddingStr, ",");
        if (parts.size() != 4) {
            throw std::runtime_error("Invalid padding size: " + std::to_string(parts.size()));
        }
        for (uint i = 0U; i < static_cast<uint>(parts.size()); i++) {
            fontFile->padding_[static_cast<glm::vec4::length_type>(i)] = std::stof(parts[i]);
        }
    }

    void Font::loadCommon(Font *fontFile, std::unordered_map<std::string, std::string> &map) {
        fontFile->imageSize_ = getIntValue("scaleW", map);
        fontFile->lineHeight_ = static_cast<float>(getIntValue("lineHeight", map));
        fontFile->base_ = static_cast<float>(getIntValue("base", map));
    }

    void Font::loadChars(Font *, std::unordered_map<std::string, std::string> &) {
        // do nothing
    }

    void Font::loadPage(Font *fontFile, std::unordered_map<std::string, std::string> &map) {
        std::string texFile = getValue("file", map);
        if (util::Files::GetDirectory(texFile).empty()) {
            const std::string fontFileDir = util::Files::GetDirectory(fontFile->getFileName());
            texFile = fontFileDir + texFile;
        }
        fontFile->textureFile_ = texFile;
    }

    void Font::loadChar(Font *fontFile, std::unordered_map<std::string, std::string> &map) {
        const float factor = 12.0f / (fontFile->getLineHeight() - fontFile->getBase());
        uint id = static_cast<uint>(getIntValue("id", map));
        const bool isSpace = id == ' ';
        const float xAdvance =
            (static_cast<float>(getIntValue("xadvance", map)) - fontFile->getHorizontalPadding()) * factor;
        if (isSpace) {
            fontFile->spaceWidth_ = xAdvance;
        }

        const glm::vec2 textureUV(
            static_cast<float>(getIntValue("x", map)) / static_cast<float>(fontFile->imageSize_),
            (static_cast<float>(fontFile->imageSize_) - static_cast<float>(getIntValue("y", map))) /
                static_cast<float>(fontFile->imageSize_));
        const float width = static_cast<float>(getIntValue("width", map));
        const float height = static_cast<float>(getIntValue("height", map));
        const glm::vec2 textureSize(static_cast<float>(width) / static_cast<float>(fontFile->imageSize_),
                                    static_cast<float>(height) / static_cast<float>(fontFile->imageSize_));
        const glm::vec2 quadSize(width * factor, height * factor);
        const glm::vec2 offset(static_cast<float>(getIntValue("xoffset", map)) * factor,
                               static_cast<float>(getIntValue("yoffset", map)) * factor);

        auto *character =
            new video::model::Letter(id, textureUV, glm::vec2(textureUV.x + textureSize.x, textureUV.y - textureSize.y),
                                     offset, quadSize, xAdvance);
        std::unordered_map<uint, video::model::Letter *>::value_type pair(id, character);
        fontFile->characterMap_.insert(pair);
    }
} // namespace soil::file
