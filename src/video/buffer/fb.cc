#include "video/buffer/fb.h"

#include <GL/gl3w.h>

#include <thread>

#include "plog/Log.h"
#include "video/texture/data.h"
#include "video/texture/manager.h"

namespace soil::video::buffer {
FrameBuffer::FrameBuffer(const glm::ivec2 size)
    : id_(0),
      colorBufferId_(0),
      depthBufferId_(0),
      stencilBufferId_(0),
      m_DepthTexture(nullptr),
      colorBufferParameter_(0),
      size_(size),
      depthType_(DepthBufferType::None),
      clearBufferBits_(0) {
  glGenFramebuffers(1, &id_);
#ifdef DEBUG
  PLOG_DEBUG << "Create framebuffer " << std::to_string(id_);
#endif
}

FrameBuffer::~FrameBuffer() {
  if (id_ == 0U) {
    return;
  }
  PLOG_INFO << "Delete framebuffer " << std::to_string(id_);
  Unload();
  glDeleteFramebuffers(1, &id_);
}

void FrameBuffer::Unload() {
  PLOG_INFO << "Unload framebuffer " << std::to_string(id_);
  if (depthBufferId_ != 0U) {
    glDeleteRenderbuffers(1, &depthBufferId_);
    depthBufferId_ = 0;
  }
  if (stencilBufferId_ != 0U) {
    glDeleteRenderbuffers(1, &stencilBufferId_);
    stencilBufferId_ = 0;
  }
  size_t textureCount = colorBufferVector_.size();
  if (m_DepthTexture != nullptr) {
    textureCount++;
  }
  std::vector<uint> ids(textureCount);
  int idx = 0;
  for (const auto* texture : colorBufferVector_) {
    ids[idx++] = texture->GetId();
  }
  if (m_DepthTexture != nullptr) {
    ids[textureCount - 1] = m_DepthTexture->GetId();
  }
  glDeleteTextures(static_cast<int>(colorBufferVector_.size()), ids.data());

  colorBufferVector_.clear();
  colorBufferParameter_.clear();
  m_DepthTexture = nullptr;
}

void FrameBuffer::Resize(const glm::ivec2 newSize) {
  if (const bool sizeChanged =
          glm::any(glm::notEqual(size_, newSize)) && newSize.x > 0;
      !sizeChanged) {
    return;
  }

  // copy values
  const DepthBufferType depthBufferType = depthType_;
  std::vector<texture::Parameter> texParameterBuffer(
      colorBufferParameter_.size());
  for (auto i = 0; i < colorBufferVector_.size(); i++) {
    texParameterBuffer[i] = colorBufferParameter_[i];
  }

  Unload();
  size_ = newSize;
  for (const auto& i : texParameterBuffer) {
    CreateColorTexture(i);
  }
  CreateDepthBuffer(depthBufferType);
}

uint FrameBuffer::CreateDepthBuffer(const DepthBufferType depthType,
                                    const int samples) {
  if (id_ == 0U) {
    return 0;
  }
  if (depthType == DepthBufferType::None) {
    return 0;
  }
  if (depthBufferId_ != 0U) {
    throw std::runtime_error("depth buffer exists");
  }

  GLenum internalFormat = GL_DEPTH_COMPONENT;
  GLenum attachment = GL_DEPTH_ATTACHMENT;
  if (depthType == DepthBufferType::Depth24Stencil8) {
    internalFormat = GL_DEPTH24_STENCIL8;
    attachment = GL_DEPTH_STENCIL_ATTACHMENT;
  }

  // Create depth buffer (renderbuffer)
  glGenRenderbuffers(1, &depthBufferId_);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId_);
  if (samples > 1) {
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat,
                                     size_.x, size_.y);
  } else {
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size_.x, size_.y);
  }

  // attach buffers
  Bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER,
                            depthBufferId_);
  CheckState();
  depthType_ = depthType;
  clearBufferBits_ = clearBufferBits_ | GL_DEPTH_BUFFER_BIT;
  return depthBufferId_;
}

texture::Texture* FrameBuffer::CreateDepthTexture(const bool onlyDepth) {
  if (id_ == 0U) {
    return nullptr;
  }
  if (depthBufferId_ != 0U) {
    throw std::runtime_error("DepthBuffer exists");
  }
  PLOG_DEBUG.printf("createDepthTexture (%dx%d, %d)", size_.x, size_.y,
                    onlyDepth);
  // Create floating point depth buffer
  const texture::Data data(nullptr, size_, GL_DEPTH_COMPONENT, GL_FLOAT);
  texture::Parameter parameter;
  parameter.MinFilter = texture::Parameter::MinFilterType::NEAREST;
  parameter.MagFilter = texture::Parameter::MagFilterType::NEAREST;
  parameter.Wrap = texture::Parameter::WrapType::CLAMP_TO_BORDER;
  parameter.Format = texture::Texture::Format::DepthComponent24;
  m_DepthTexture = texture::Manager::GenerateTexture2D(
      data, "depthTexture_" + std::to_string(id_), parameter);
  depthBufferId_ = m_DepthTexture->GetId();

  Bind();
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBufferId_, 0);

  if (onlyDepth) {
    // No color output in the bound framebuffer, only depth.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }
  CheckState();
  return m_DepthTexture;
}

uint FrameBuffer::CreateColorBuffer(const int samples) {
  if (id_ == 0U) {
    return 0;
  }
  if (colorBufferId_ != 0U) {
    throw std::runtime_error("ColorBuffer already exists");
  }

  constexpr GLenum internalFormat = GL_RGBA16F;
  constexpr GLenum attachment = GL_COLOR_ATTACHMENT0;

  // Create depth buffer (renderbuffer)
  glGenRenderbuffers(1, &colorBufferId_);
  glBindRenderbuffer(GL_RENDERBUFFER, colorBufferId_);
  if (samples > 1) {
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat,
                                     size_.x, size_.y);
  } else {
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size_.x, size_.y);
  }
  // attach buffers
  Bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER,
                            colorBufferId_);
  CheckState();
  clearBufferBits_ = clearBufferBits_ | GL_COLOR_BUFFER_BIT;
  return colorBufferId_;
}

texture::Texture* FrameBuffer::CreateColorTexture() {
  texture::Parameter params;
  params.MinFilter = texture::Parameter::MinFilterType::LINEAR;
  params.MagFilter = texture::Parameter::MagFilterType::LINEAR;
  params.Format = texture::Texture::Format::RGBA16F;
  return CreateColorTexture(params);
}

texture::Texture* FrameBuffer::CreateColorTexture(
    const texture::Parameter& parameter) {
  if (id_ == 0U) {
    return nullptr;
  }
  const texture::Data data(nullptr, size_, GL_RGB, GL_UNSIGNED_BYTE);
  auto* texture = texture::Manager::GenerateTexture2D(
      data, "colorTexture_" + std::to_string(id_), parameter);
  Bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0 + colorBufferVector_.size(),
                         GL_TEXTURE_2D, texture->GetId(), 0);
  CheckState();
  texture::Texture* sharedTexture(texture);
  colorBufferVector_.push_back(sharedTexture);
  colorBufferParameter_.push_back(parameter);
  if (colorBufferVector_.size() > 1) {
    // tell OpenGL which color attachments we'll Use (of this framebuffer) for
    // rendering
    std::vector<uint> attachments(colorBufferVector_.size());
    for (auto i = 0; i < colorBufferVector_.size(); ++i) {
      attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(static_cast<int>(colorBufferVector_.size()),
                  attachments.data());
  } else {
    constexpr GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
  }
  CheckState();
  clearBufferBits_ = clearBufferBits_ | GL_COLOR_BUFFER_BIT;
  return sharedTexture;
}

void FrameBuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id_); }

void FrameBuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::Clear() const { glClear(clearBufferBits_); }

texture::Texture* FrameBuffer::GetColorBufferTexture(const int index) const {
  if (index >= colorBufferVector_.size()) {
    throw std::runtime_error("Index out of bounds: index(" +
                             std::to_string(index) + ") size(" +
                             std::to_string(colorBufferVector_.size()) + ")");
  }
  return colorBufferVector_.at(index);
}

texture::Texture* FrameBuffer::GetDepthTexture() const {
  return m_DepthTexture;
}

uint FrameBuffer::GetDepthBufferId() const { return depthBufferId_; }

uint FrameBuffer::GetId() const { return id_; }

glm::ivec2 FrameBuffer::GetSize() const { return size_; }

void FrameBuffer::CheckState() {
  const GLenum state = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (state == GL_FRAMEBUFFER_COMPLETE) {
    return;
  }
  switch (state) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      throw std::runtime_error("INCOMPLETE_ATTACHMENT");
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      throw std::runtime_error("INCOMPLETE_MISSING_ATTACHMENT");
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      throw std::runtime_error("INCOMPLETE_DRAW_BUFFER");
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      throw std::runtime_error("INCOMPLETE_READ_BUFFER");
    case GL_FRAMEBUFFER_UNSUPPORTED:
      throw std::runtime_error("UNSUPPORTED");
    default:
      throw std::runtime_error("UNKNOWN");
  }
}

void FrameBuffer::BlitTo(const FrameBuffer& otherFbo) const {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, otherFbo.GetId());
  glBindFramebuffer(GL_READ_FRAMEBUFFER, GetId());
  glBlitFramebuffer(0, 0, GetSize().x, GetSize().y, 0, 0, otherFbo.GetSize().x,
                    otherFbo.GetSize().y,
                    GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
  Unbind();
}
}  // namespace soil::video::buffer
