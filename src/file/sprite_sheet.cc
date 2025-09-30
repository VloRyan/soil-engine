#include "file/sprite_sheet.h"

#include <fstream>

#include "util/files.h"

namespace soil::file {

int SpriteSheet::TileIndex(const std::string& name) const {
  if (name.empty()) {
    return -1;
  }
  return Tiles.at(name);
}

const Sequence& SpriteSheet::GetSequence(const std::string& name) const {
  return Sequences.at(name);
}

SpriteSheet SpriteSheet::Load(const std::string& file) {
  std::ifstream f(file);
  if (!f.is_open()) {
    throw std::invalid_argument("Invalid spritesheet file");
  }
  const auto data = json::parse(f);
  if (!data.contains("type") || data["type"] != "spritesheet") {
    throw std::invalid_argument("Invalid spritesheet file");
  }
  if (!data.contains("version") || data["version"] != "1.0.0") {
    throw std::invalid_argument("Unknown spritesheet file version");
  }
  auto s = data.get<SpriteSheet>();
  s.Path = util::Files::GetDirectory(file);
  return s;
}

void SpriteSheet::Save(const std::string& file) {
  const json j = *this;
  std::ofstream f(file);
  f << j.dump(4) << std::endl;
  f.close();
}

std::string SpriteSheet::GetTextureFile() {
  if (util::Files::IsAbsolute(TextureFileName)) {
    return TextureFileName;
  }
  return std::filesystem::path(Path) / TextureFileName;
}

}  // namespace soil::file
