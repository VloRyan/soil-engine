#ifndef SOIL_VIDEO_TEXTURE_TEXTURE_H
#define SOIL_VIDEO_TEXTURE_TEXTURE_H
#include <GL/glcorearb.h>

#include <glm/glm.hpp>
#include <string>

#include "base.h"

namespace soil::video::render {
class State;
}

namespace soil::video::texture {
class Texture final {
  friend class Manager;
  friend class Loader;
  friend class render::State;

 public:
  enum Format : std::uint16_t {
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    RGBA16F = GL_RGBA16F,
    sRGB = GL_SRGB,
    sRGBA = GL_SRGB_ALPHA,
    DepthComponent = GL_DEPTH_COMPONENT,
    DepthComponent16 = GL_DEPTH_COMPONENT16,
    DepthComponent24 = GL_DEPTH_COMPONENT24,
    DepthComponent32 = GL_DEPTH_COMPONENT32,
  };

  enum Type : std::uint16_t {
    Texture2D = GL_TEXTURE_2D,
    TextureArray2D = GL_TEXTURE_2D_ARRAY,
    CubeMap = GL_TEXTURE_CUBE_MAP,
  };

  Texture(uint id, std::string name, glm::uvec2 size, Type type = Texture2D,
          Format format = sRGBA);

  ~Texture();

  /**
   * Returns the filename of the texture.
   * @return
   */
  std::string GetName();

  /**
   * Returns the OpenGl id.
   * @return OpenGl Id
   */
  [[nodiscard]] uint GetId() const;

  [[nodiscard]] Type GetType() const;

  [[nodiscard]] Format GetFormat() const;

  [[nodiscard]] glm::uvec2 GetSize() const;

  [[nodiscard]] uint64_t GetHandle() const;

  [[nodiscard]] uint GetBufferIndex() const;

  [[nodiscard]] bool IsResident() const;

  void SetResident(bool);

  [[nodiscard]] char GetSlot() const;

 protected:
  void free();

 private:
  uint id_;
  char unit_;
  uint64_t handle_;
  bool isResident_;
  uint bufferIndex_;
  std::string name_;
  Type type_;
  Format format_;
  glm::uvec2 size_;
};
}  // namespace soil::video::texture
#endif
