#ifndef SOIL_FILE_SPRITE_SHEET_H
#define SOIL_FILE_SPRITE_SHEET_H

#include <nlohmann/json.hpp>
#include <string>

#include "video/texture/animation.h"
#include "video/texture/texture.h"

using json = nlohmann::json;

namespace soil::file {
struct SpriteSheet {
  std::string TextureFileName;
  int FramesPerDim{0};
  std::unordered_map<std::string, int> Frames;
  std::unordered_map<std::string, std::vector<video::texture::Animation::Frame>>
      Animations;
  std::string Path;

  int FrameByName(const std::string& name) const;

  const std::vector<video::texture::Animation::Frame>* AnimationByName(
      const std::string& name) const;

  static SpriteSheet Load(const std::string& file);

  std::string GetTextureFile() const;

  void Save(const std::string& file);

  friend bool operator==(const SpriteSheet& lhs, const SpriteSheet& rhs) {
    return lhs.TextureFileName == rhs.TextureFileName &&
           lhs.FramesPerDim == rhs.FramesPerDim && lhs.Frames == rhs.Frames &&
           lhs.Animations == rhs.Animations;
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
  if (!p.Animations.empty()) {
    auto jAnimations = json{};
    for (auto& seqPair : p.Animations) {
      auto jFrames = json{};
      for (auto i = 0; i < seqPair.second.size(); i++) {
        jFrames[i] = json{
            {"tileIndex", seqPair.second[i].Index},  //
            {"duration", seqPair.second[i].Duration},
        };
      }
      jAnimations[seqPair.first] = jFrames;
    }
    j["animations"] = jAnimations;
  }
}

static void from_json(const json& j, SpriteSheet& p) {
  j.at("textureFileName").get_to(p.TextureFileName);
  j.at("framesPerDim").get_to(p.FramesPerDim);
  if (j.contains("frames")) {
    j.at("frames").get_to(p.Frames);
  }
  if (j.contains("animations")) {
    for (auto& jAni : j.at("animations").items()) {
      auto frames = std::vector<video::texture::Animation::Frame>();
      for (auto jFrame : jAni.value()) {
        frames.push_back({
            .Index = jFrame.at("tileIndex").get<int>(),
            .Duration = jFrame.at("duration").get<int>(),
        });
      }
      p.Animations.insert({jAni.key(), frames});
    }
  }
}
}  // namespace soil::file

#endif
