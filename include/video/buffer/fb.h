#ifndef SOIL_VIDEO_BUFFER_FRAMEBUFFER_H
#define SOIL_VIDEO_BUFFER_FRAMEBUFFER_H
#include <vector>

#include "video/texture/parameter.h"
#include "video/texture/texture.h"

#define ALL_BUFFER_BIT \
  (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)
#define COLOR_AND_DEPTH_BUFFER_BIT (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

namespace soil::video::buffer {
class FrameBuffer final {
 public:
  enum class DepthBufferType : std::uint8_t {
    None = 0,
    Depth,
    Depth24Stencil8
  };
  struct RenderBufferDefinition {
    uint Id{0U};
    GLenum InternalFormat;
    int Samples{0};
  };

  explicit FrameBuffer(glm::ivec2 size);

  ~FrameBuffer();

  void Resize(glm::ivec2 newSize);

  uint CreateDepthBuffer(DepthBufferType = DepthBufferType::Depth,
                         int samples = 0);
  uint CreateColorBuffer(int samples = 0);
  [[nodiscard]] texture::Texture* CreateDepthTexture(bool onlyDepth = false);
  [[nodiscard]] texture::Texture* CreateColorTexture(
      const texture::Parameter& parameter = {});

  void BlitTo(const FrameBuffer& otherFbo) const;

  void Bind() const;

  static void Unbind();

  void Clear() const;

  [[nodiscard]] texture::Texture* GetColorAttachmentTexture(int index) const;

  [[nodiscard]] texture::Texture* GetDepthTexture() const;

  [[nodiscard]] const RenderBufferDefinition& GetDepthBuffer() const;

  [[nodiscard]] glm::ivec2 GetSize() const;

 protected:
  [[nodiscard]] uint GetId() const;
  static void CheckState();
  void Unload();

  void ResizeRenderBuffer(const RenderBufferDefinition& depthBuffer,
                          glm::ivec2 size);

  int toTexture2DDataFormat(texture::Texture::Format format);

 private:
  uint id_;
  RenderBufferDefinition depthBuffer_;
  RenderBufferDefinition colorBuffer_;
  uint stencilBufferId_;
  texture::Texture* depthTexture_;
  std::vector<texture::Texture*> colorAttachmentTextures_;
  glm::ivec2 size_;
  int clearBufferBits_;
};
}  // namespace soil::video::buffer
#endif
