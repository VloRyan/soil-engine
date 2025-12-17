#include "video/buffer/fb.h"

#include <GL/gl3w.h>

#include <thread>

#include "plog/Log.h"
#include "video/texture/data.h"
#include "video/texture/manager.h"

namespace soil::video::buffer {
FrameBuffer::FrameBuffer(const glm::ivec2 size)
    : id_(0),
      stencilBufferId_(0),
      depthTexture_(nullptr),
      size_(size),
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
  Bind();
  if (depthBuffer_.Id != 0U) {
    glDeleteRenderbuffers(1, &depthBuffer_.Id);
  }
  if (stencilBufferId_ != 0U) {
    glDeleteRenderbuffers(1, &stencilBufferId_);
  }

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  for (auto i = 0; i < colorAttachmentTextures_.size(); ++i) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                           GL_TEXTURE_2D, 0, 0);
    delete colorAttachmentTextures_[i];
  }
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
  delete depthTexture_;

  depthBuffer_.Id = 0U;
  stencilBufferId_ = 0U;
  colorAttachmentTextures_.clear();
  depthTexture_ = nullptr;
}

void FrameBuffer::Resize(const glm::ivec2 newSize) {
  if (const bool sizeChanged =
          glm::any(glm::notEqual(size_, newSize)) && newSize.x > 0;
      !sizeChanged) {
    return;
  }
  Bind();
  if (depthBuffer_.Id != 0U) {
    ResizeRenderBuffer(depthBuffer_, newSize);
  }
  if (depthTexture_ != nullptr) {
    const texture::Data data(nullptr, size_, GL_DEPTH_COMPONENT, GL_FLOAT);
    texture::Manager::ResizeTexture2D(depthTexture_->GetId(), data,
                                      depthTexture_->GetFormat());
  }
  if (colorBuffer_.Id != 0U) {
    ResizeRenderBuffer(colorBuffer_, newSize);
  }
  texture::Data data(nullptr, newSize, 0, GL_UNSIGNED_BYTE);
  for (auto* colorTexture : colorAttachmentTextures_) {
    data.Format = toTexture2DDataFormat(colorTexture->GetFormat());
    texture::Manager::ResizeTexture2D(colorTexture->GetId(), data,
                                      colorTexture->GetFormat());
  }
  size_ = newSize;
  Unbind();
}

uint FrameBuffer::CreateDepthBuffer(const DepthBufferType depthType,
                                    const int samples) {
  if (id_ == 0U || depthType == DepthBufferType::None) {
    return 0;
  }
  if (depthTexture_ != nullptr || depthBuffer_.Id != 0U) {
    throw std::runtime_error("depth attachment already exists");
  }

  GLenum internalFormat = GL_DEPTH_COMPONENT;
  GLenum attachment = GL_DEPTH_ATTACHMENT;
  if (depthType == DepthBufferType::Depth24Stencil8) {
    internalFormat = GL_DEPTH24_STENCIL8;
    attachment = GL_DEPTH_STENCIL_ATTACHMENT;
  }

  // Create depth buffer
  glGenRenderbuffers(1, &depthBuffer_.Id);
  depthBuffer_.InternalFormat = internalFormat;
  depthBuffer_.Samples = samples;
  ResizeRenderBuffer(depthBuffer_, size_);

  // attach buffers
  Bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER,
                            depthBuffer_.Id);
  CheckState();
  clearBufferBits_ = clearBufferBits_ | GL_DEPTH_BUFFER_BIT;
  Unbind();
  return depthBuffer_.Id;
}

uint FrameBuffer::CreateColorBuffer(const int samples) {
  if (id_ == 0U) {
    return 0;
  }
  if (colorBuffer_.Id != 0U) {
    throw std::runtime_error("ColorBuffer already exists");
  }

  constexpr GLenum attachment = GL_COLOR_ATTACHMENT0;

  glGenRenderbuffers(1, &colorBuffer_.Id);
  colorBuffer_.InternalFormat = GL_RGBA16F;
  colorBuffer_.Samples = samples;
  ResizeRenderBuffer(colorBuffer_, size_);

  // attach buffers
  Bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER,
                            colorBuffer_.Id);
  CheckState();
  clearBufferBits_ = clearBufferBits_ | GL_COLOR_BUFFER_BIT;
  return colorBuffer_.Id;
}

void FrameBuffer::ResizeRenderBuffer(const RenderBufferDefinition& depthBuffer,
                                     glm::ivec2 size) {
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer.Id);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, depthBuffer.Samples,
                                   depthBuffer.InternalFormat, size.x, size.y);
}

texture::Texture* FrameBuffer::CreateDepthTexture(const bool onlyDepth) {
  if (id_ == 0U) {
    return nullptr;
  }
  if (depthTexture_ != nullptr || depthBuffer_.Id != 0U) {
    throw std::runtime_error("depth attachment already exists");
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
  depthTexture_ = texture::Manager::GenerateTexture2D(
      data, "depthTexture_" + std::to_string(id_), parameter);
  Bind();
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBuffer_.Id, 0);

  if (onlyDepth) {
    // No color output in the bound framebuffer, only depth.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }
  CheckState();
  return depthTexture_;
}

texture::Texture* FrameBuffer::CreateColorTexture(
    const texture::Parameter& parameter) {
  if (id_ == 0U) {
    return nullptr;
  }

  const texture::Data data(nullptr, size_,
                           toTexture2DDataFormat(parameter.Format),
                           GL_UNSIGNED_BYTE);
  auto* texture = texture::Manager::GenerateTexture2D(
      data, "colorTexture_" + std::to_string(id_), parameter);
  Bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0 + colorAttachmentTextures_.size(),
                         GL_TEXTURE_2D, texture->GetId(), 0);
  CheckState();
  colorAttachmentTextures_.push_back(texture);
  //  tell OpenGL which color attachments we'll use (of this framebuffer) for
  //  rendering
  std::vector<uint> attachments(colorAttachmentTextures_.size());
  for (auto i = 0; i < colorAttachmentTextures_.size(); ++i) {
    attachments[i] = GL_COLOR_ATTACHMENT0 + i;
  }
  glDrawBuffers(static_cast<int>(colorAttachmentTextures_.size()),
                attachments.data());
  CheckState();
  clearBufferBits_ = clearBufferBits_ | GL_COLOR_BUFFER_BIT;
  Unbind();
  return texture;
}

void FrameBuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id_); }

void FrameBuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::Clear() const { glClear(clearBufferBits_); }

texture::Texture* FrameBuffer::GetColorAttachmentTexture(
    const int index) const {
  if (index >= colorAttachmentTextures_.size()) {
    throw std::runtime_error(
        "Index out of bounds: index(" + std::to_string(index) + ") size(" +
        std::to_string(colorAttachmentTextures_.size()) + ")");
  }
  return colorAttachmentTextures_.at(index);
}

texture::Texture* FrameBuffer::GetDepthTexture() const { return depthTexture_; }

const FrameBuffer::RenderBufferDefinition& FrameBuffer::GetDepthBuffer() const {
  return depthBuffer_;
}

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
int FrameBuffer::toTexture2DDataFormat(texture::Texture::Format format) {
  switch (format) {
    case texture::Texture::RGB:
    case texture::Texture::sRGB:
      return GL_RGB;
    case texture::Texture::RGBA:
    case texture::Texture::RGBA8:
    case texture::Texture::RGBA16F:
    case texture::Texture::sRGBA:
    case texture::Texture::sRGBA8:
      return GL_RGBA;
    default:
      throw std::runtime_error("unknown format: " + std::to_string(format));
  }
}

}  // namespace soil::video::buffer
