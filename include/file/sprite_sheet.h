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
  int FramesPerDim{0};
  std::unordered_map<std::string, int> Frames;
  std::unordered_map<std::string, Sequence> Sequences;
  std::string Path;

  int FrameByName(const std::string& name) const;

  const Sequence& SequenceByName(const std::string& name) const;

  static SpriteSheet Load(const std::string& file);

  std::string GetTextureFile() const;

  void Save(const std::string& file);

  friend bool operator==(const SpriteSheet& lhs, const SpriteSheet& rhs) {
    return lhs.TextureFileName == rhs.TextureFileName &&
           lhs.FramesPerDim == rhs.FramesPerDim && lhs.Frames == rhs.Frames &&
           lhs.Sequences == rhs.Sequences;
  }

  friend bool operator!=(const SpriteSheet& lhs, const SpriteSheet& rhs) {
    return !(lhs == rhs);
  }
};

static void to_json(json& j, const SpriteSheet& p) {
  j = json{
      {"type", "spritesheet"},                 //
      {"version", "1.0.0"},                    //
      {"textureFileName", p.TextureFileName},  //
      {"framesPerDim", p.FramesPerDim},        //
  };
  if (!p.Frames.empty()) {
    j["frames"] = p.Frames;
  }
  if (!p.Sequences.empty()) {
    j["sequences"] = p.Sequences;
  }
}

static void from_json(const json& j, SpriteSheet& p) {
  j.at("textureFileName").get_to(p.TextureFileName);
  j.at("framesPerDim").get_to(p.FramesPerDim);
  if (j.contains("frames")) {
    j.at("frames").get_to(p.Frames);
  }
  if (j.contains("sequences")) {
    j.at("sequences").get_to(p.Sequences);
  }
}
}  // namespace soil::file

#endif
