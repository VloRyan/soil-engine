#ifndef VIDEO_FRAMEBUFFER_H
#define VIDEO_FRAMEBUFFER_H
#include <vector>
#include "base.h"
#include "video/texture/texture.h"
#include "video/texture/parameter.h"

#define ALL_BUFFER_BIT GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
#define COLOR_AND_DEPTH_BUFFER_BIT GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT

namespace video::buffer {
 class FrameBuffer {
 public:
  enum class DepthBufferType : std::uint8_t {
   None = 0,
   Depth,
   Depth24Stencil8
  };

  explicit FrameBuffer(glm::ivec2 size);

  virtual ~FrameBuffer();

  void Resize(glm::ivec2 newSize);

  uint CreateDepthBuffer(DepthBufferType = DepthBufferType::Depth, int samples = 1);

  uint CreateColorBuffer(int samples = 1);

  [[nodiscard]] texture::Texture* CreateDepthTexture(bool onlyDepth = false);

  [[nodiscard]] texture::Texture* CreateColorTexture();

  [[nodiscard]] texture::Texture* CreateColorTexture(const texture::Parameter &parameter);

  void BlitTo(const FrameBuffer &otherFbo) const;

  void Bind() const;

  static void Unbind();

  void Clear() const;

  [[nodiscard]] texture::Texture* GetColorBufferTexture(int index) const;

  [[nodiscard]] texture::Texture* GetDepthTexture() const;

  [[nodiscard]] uint GetDepthBufferId() const;

  [[nodiscard]] glm::ivec2 GetSize() const;

 protected:
  [[nodiscard]] uint GetId() const;

  static void CheckState();

  void Unload();

 private:
  uint id_;
  uint colorBufferId_;
  uint depthBufferId_;
  uint stencilBufferId_;
  texture::Texture *m_DepthTexture;
  std::vector<texture::Texture *> colorBufferVector_;
  std::vector<texture::Parameter> colorBufferParameter_;
  glm::ivec2 size_;
  DepthBufferType depthType_;
  int clearBufferBits_;
 };
}
#endif /* VIDEO_FRAMEBUFFER_H */

