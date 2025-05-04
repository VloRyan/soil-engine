#ifndef ENGINE_VIDEO_MODEL_CHARACTER_H_
#define ENGINE_VIDEO_MODEL_CHARACTER_H_

#include "base.h"
#include <glm/glm.hpp>

namespace video::model {
struct Letter {
  uint Id;
  glm::vec2 TextureCoord;
  glm::vec2 MaxTextureCoord;
  glm::vec2 Offset;
  glm::vec2 Size;
  float XAdvance;

  [[nodiscard]] bool isPrintable() const;
};
} // namespace video::model

#endif // ENGINE_VIDEO_MODEL_CHARACTER_H_
