#ifndef SOIL_VIDEO_RENDER_STATE_H
#define SOIL_VIDEO_RENDER_STATE_H
#include <video/buffer/fb.h>
#include <video/buffer/ubo.h>

#include <functional>
#include <optional>

#include "types.hpp"

namespace soil::video::render {
struct StateDef {
  std::optional<bool> Blend{};
  std::optional<bool> DepthTest{};
  std::optional<bool> StencilTest{};
  std::optional<bool> ScissorTest{};
  std::optional<render::DepthFunc> DepthFunc{DepthFunc::Less};
  std::optional<glm::vec4> ClearColor{};
  std::optional<buffer::FrameBuffer*> Framebuffer;
  std::optional<Rect> ViewPort;
  std::optional<Rect> Scissor;
  bool operator==(const StateDef& rhs) const;
  bool operator!=(const StateDef& rhs) const;
  bool operator<(const StateDef& rhs) const;
  bool operator>(const StateDef& rhs) const;
  bool operator<=(const StateDef& rhs) const;
  bool operator>=(const StateDef& rhs) const;
};

class State final {
 public:
  State();

  virtual ~State() = default;

  void Init();

  void SetDepthTest(bool depthTest);

  [[nodiscard]] bool GetDepthTest() const;

  void SetDepthFunc(DepthFunc depthFunc);

  [[nodiscard]] DepthFunc GetDepthFunc() const;

  [[nodiscard]] bool GetBlend() const;

  void SetBlend(bool blend);

  [[nodiscard]] bool IsStencilTest() const;

  void SetStencilTest(bool stencilTest);

  [[nodiscard]] bool IsScissorTest() const;

  void SetScissorTest(bool scissorTest);

  void SetScissor(const Rect& rect);

  Rect GetScissor() const;

  [[nodiscard]] buffer::FrameBuffer* GetFramebuffer() const;

  void SetFramebuffer(buffer::FrameBuffer* framebuffer);

  [[nodiscard]] Rect GetViewPort() const;

  void SetViewPort(const Rect& rect);

  void Apply(const StateDef& def);
  StateDef Pop();

  void WriteUbo(int target,
                const std::function<void(buffer::Cursor& cursor)>& writeFunc);
#ifdef DEBUG
  void ResetChangeCounter();
#endif
  void RegisterUbo(int target, buffer::UniformBufferObject* ubo);

  [[nodiscard]] virtual int GetChanges() const;

  /**
   * Sets the texture to the next free texture unit. Default texture unit 0 is
   * left free for loading textures.
   * @param texture to set
   * @return OpenGL texture slot (1 ... GL_MAX_TEXTURE_IMAGE_UNITS)
   */
  char SetTexture(texture::Texture& texture);

  void SetTexture(byte textureUnit, texture::Texture& texture);

  void SetTexture(GLenum target, byte textureUnit, texture::Texture& texture);

  texture::Texture* GetTexture(byte textureUnit);

  [[nodiscard]] virtual int GetMaxImageUnits() const;

  void Clear(BufferBitDescription bits);

  const glm::vec4& GetClearColor() const;
  void SetClearColor(const glm::vec4& clearColor);

 private:
  bool depthTest_;
  DepthFunc depthFunc_;
  bool stencilTest_;
  bool scissorTest_;
  bool blend_;
  glm::vec4 clearColor_;

  int changes_;
  int maxImageUnits_;

  buffer::UniformBufferObject* uboMatrices_;
  std::unordered_map<int, buffer::UniformBufferObject*> uboMap_;
  buffer::FrameBuffer* framebuffer_;
  Rect viewPort_;
  Rect scissor_;
  std::vector<texture::Texture*> textureUnits_;
};
}  // namespace soil::video::render

#endif
