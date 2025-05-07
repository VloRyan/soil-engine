#ifndef SOIL_FILE_FONT_H_
#define SOIL_FILE_FONT_H_

#include <string>
#include <unordered_map>

#include "video/model/letter.h"

namespace soil::file {
    class Font final {
    public:
        friend class Font_Loader;

        ~Font();

        glm::vec4 getPadding() const;

        float getSpaceWidth() const;

        video::model::Letter *getCharacter(uint id);

        std::string getTextureFile() const;

        float getHorizontalPadding();

        float getVerticalPadding();

        uint getImageSize() const;

        std::string getFileName() const;

        float getLineHeight() const;

        float getBase() const;

        static Font *Load(std::string fontFilePath);

    protected:
        explicit Font(std::string fileName);

        static void mapValues(std::string line, std::unordered_map<std::string, std::string> &map);

        static int getIntValue(const std::string &key, std::unordered_map<std::string, std::string> &map);

        static std::string getValue(const std::string &key, std::unordered_map<std::string, std::string> &map);

        static void loadInfos(Font *fontFile, std::unordered_map<std::string, std::string> &map);

        static void loadCommon(Font *fontFile, std::unordered_map<std::string, std::string> &map);

        static void loadChars(Font *fontFile, std::unordered_map<std::string, std::string> &map);

        static void loadPage(Font *fontFile, std::unordered_map<std::string, std::string> &map);

        static void loadChar(Font *fontFile, std::unordered_map<std::string, std::string> &map);

    private:
        std::string fileName_;
        float lineHeight_;
        float base_;
        float spaceWidth_;
        glm::vec4 padding_;
        std::unordered_map<uint, video::model::Letter *> characterMap_;
        std::string textureFile_;
        uint imageSize_;
    };
} // namespace soil::file

#endif // SOIL_FILE_FONT_H_
