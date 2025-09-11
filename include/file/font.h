#ifndef SOIL_FILE_FONT_H
#define SOIL_FILE_FONT_H

#include <debug/vector>
#include <string>
#include <unordered_map>

#include "video/model/letter.h"

namespace soil::file {


    struct Font {
        struct Character {
            int Id {-1};
            glm::ivec2 TextureCoord {0, 0};
            glm::ivec2 Offset {0, 0};
            glm::ivec2 Size {0, 0};
            int XAdvance {0};

            [[nodiscard]] bool IsPrintable() const;

            friend bool operator==(const Character& lhs, const Character& rhs) {
                return lhs.Id == rhs.Id && lhs.TextureCoord == rhs.TextureCoord && lhs.Offset == rhs.Offset &&
                    lhs.Size == rhs.Size && lhs.XAdvance == rhs.XAdvance;
            }

            friend bool operator!=(const Character& lhs, const Character& rhs) {
                return !(lhs == rhs);
            }
        };

        struct Kerning {
            int First;
            int Second;
            int Amount;

            friend bool operator==(const Kerning& lhs, const Kerning& rhs) {
                return lhs.First == rhs.First && lhs.Second == rhs.Second && lhs.Amount == rhs.Amount;
            }

            friend bool operator!=(const Kerning& lhs, const Kerning& rhs) {
                return !(lhs == rhs);
            }
        };

        std::string FileName {};
        int LineHeight {0};
        int Base {0};
        glm::ivec4 Padding {0, 0, 0, 0};
        std::unordered_map<int, Character> Characters {};
        std::vector<Kerning> Kernings {};
        std::string TextureFileName {};
        glm::ivec2 ImageSize {};
        static Font* Load(const std::string& filePath);

    private:
        enum class LineType : std::int8_t { Info = 0, Common, Page, Chars, Char, Kernings, Kerning, Unknown };
        static LineType parseLineValues(const std::string& line, std::unordered_map<std::string, std::string>& values);
        static int getIntValue(const std::string& key, std::unordered_map<std::string, std::string>& values);
        static std::string getValue(const std::string& key, std::unordered_map<std::string, std::string>& values);
        static void parseInfos(Font* fontFile, std::unordered_map<std::string, std::string>& values);
        static void parseCommon(Font* fontFile, std::unordered_map<std::string, std::string>& values);
        static void parsePage(Font* fontFile, std::unordered_map<std::string, std::string>& values);
        static void parseChar(Font* fontFile, std::unordered_map<std::string, std::string>& values);
        static void parseKerning(Font* fontFile, std::unordered_map<std::string, std::string>& values);
    };

    /*
        class Font final {
        public:
            struct Character {
                uint Id;
                glm::vec2 TextureCoord;
                glm::vec2 MaxTextureCoord;
                glm::vec2 Offset;
                glm::vec2 Size;
                float XAdvance;

                [[nodiscard]] bool IsPrintable() const;
            };

            ~Font();

            glm::vec4 GetPadding() const;

            float GetSpaceWidth() const;

            video::model::Letter* GetLetter(std::uint32_t code);

            std::string GetTextureFile() const;

            float GetHorizontalPadding();

            float GetVerticalPadding();

            uint GetImageSize() const;

            std::string GetFileName() const;

            float GetLineHeight() const;

            float GetBase() const;

            static Font* Load(std::string fontFilePath);

        protected:
            explicit Font(std::string fileName);

            static void mapValues(std::string line, std::unordered_map<std::string, std::string>& map);

            static int GetIntValue(const std::string& key, std::unordered_map<std::string, std::string>& map);

            static std::string getValue(const std::string& key, std::unordered_map<std::string, std::string>& map);

            static void loadInfos(Font* fontFile, std::unordered_map<std::string, std::string>& map);

            static void loadCommon(Font* fontFile, std::unordered_map<std::string, std::string>& map);

            static void loadChars(Font* fontFile, std::unordered_map<std::string, std::string>& map);

            static void loadPage(Font* fontFile, std::unordered_map<std::string, std::string>& map);

            static void loadChar(Font* fontFile, std::unordered_map<std::string, std::string>& map);

        private:
            std::string fileName_;
            float lineHeight_;
            float base_;
            float spaceWidth_;
            glm::vec4 padding_;
            glm::vec2 perPixelSize_;
            std::unordered_map<std::uint32_t, video::model::Letter*> characterMap_;
            std::string textureFile_;
            uint imageSize_;
        }; */
} // namespace soil::file

#endif
