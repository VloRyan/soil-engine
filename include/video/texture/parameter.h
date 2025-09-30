#ifndef SOIL_VIDEO_TEXTURE_PARAMETER_H
#define SOIL_VIDEO_TEXTURE_PARAMETER_H

#include "GL/glcorearb.h"
#include "texture.h"

namespace soil::video::texture {
struct Parameter final {
  enum class WrapType : GLint {
    REPEAT = GL_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT
  };

  enum class MagFilterType : GLint {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
  };

  enum class MinFilterType : GLint {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
  };

  WrapType Wrap{WrapType::REPEAT};
  MinFilterType MinFilter{MinFilterType::LINEAR_MIPMAP_LINEAR};
  MagFilterType MagFilter{MagFilterType::LINEAR};
  Texture::Format Format{Texture::Format::sRGBA};
};
}  // namespace soil::video::texture
#endif
