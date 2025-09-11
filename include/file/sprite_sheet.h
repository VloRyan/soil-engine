#ifndef SOIL_FILE_SPRITE_SHEET_H
#define SOIL_FILE_SPRITE_SHEET_H

#include <nlohmann/json.hpp>
#include <string>

#include "sequence.hpp"
#include "video/texture/texture.h"


using json = nlohmann::json;

namespace soil::file {

    struct SpriteSheet {
        std::string TextureFileName;
        video::texture::Texture* Texture {nullptr};
        int TilesPerDim {0};
        std::unordered_map<std::string, int> Tiles;
        std::unordered_map<std::string, Sequence> Sequences;
        std::string Path;

        int TileIndex(const std::string& name) const;
        const Sequence& GetSequence(const std::string& name) const;

        static SpriteSheet Load(const std::string& file);
        void Save(const std::string& file);

        std::string GetTextureFile();

        friend bool operator==(const SpriteSheet& lhs, const SpriteSheet& rhs) {
            return lhs.TextureFileName == rhs.TextureFileName && lhs.TilesPerDim == rhs.TilesPerDim &&
                lhs.Tiles == rhs.Tiles && lhs.Sequences == rhs.Sequences;
        }

        friend bool operator!=(const SpriteSheet& lhs, const SpriteSheet& rhs) {
            return !(lhs == rhs);
        }
    };

    static void to_json(json& j, const SpriteSheet& p) {
        json jMap(p.Tiles);
        j = json {
            {"type", "spritesheet"}, //
            {"version", "1.0.0"}, //
            {"textureFileName", p.TextureFileName}, //
            {"tilesPerDim", p.TilesPerDim}, //
            {"tiles", json(p.Tiles)}, //
            {"sequences", json(p.Sequences)} //
        };
    }

    static void from_json(const json& j, SpriteSheet& p) {
        j.at("textureFileName").get_to(p.TextureFileName);
        j.at("tilesPerDim").get_to(p.TilesPerDim);
        j.at("tiles").get_to(p.Tiles);
        if (j.contains("sequences")) {
            j.at("sequences").get_to(p.Sequences);
        }
    }
} // namespace soil::file


#endif
