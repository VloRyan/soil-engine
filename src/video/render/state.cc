
#include "video/render/state.h"

#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <plog/Log.h>

#include "video/mesh/data.h"
#include "window.h"

namespace soil::video::render {
State::State()
    : depthTest_(false),
      depthFunc_(DepthFunc::Less),
      stencilTest_(false),
      scissorTest_(false),
      blend_(false),
      changes_(0),
      maxImageUnits_(0),
      uboMatrices_(nullptr),
      framebuffer_(nullptr),
      viewPort_({}),
      scissor_({}) {}

void State::Apply(const StateDef& def) {
  if (def.Blend.has_value()) {
    this->SetBlend(def.Blend.value());
  }
  if (def.DepthTest.has_value()) {
    this->SetDepthTest(def.DepthTest.value());
  }
  if (def.StencilTest.has_value()) {
    this->SetStencilTest(def.StencilTest.value());
  }
}

void State::WriteUbo(
    const int target,
    const std::function<void(buffer::Cursor* cursor)>& writeFunc) {
  const auto itr = uboMap_.find(target);
  if (itr == uboMap_.end()) {
    throw std::runtime_error("Cannot find Ubo for target: " +
                             std::to_string(target));
  }
  auto* cursor = itr->second->GetCursor();
  writeFunc(cursor);
  itr->second->Flush();
#ifdef DEBUG
  changes_++;
#endif
}

void State::Init() {
  blend_ = glIsEnabled(GL_BLEND);
  depthTest_ = glIsEnabled(GL_DEPTH_TEST);
  auto value = 0;
  glGetIntegerv(GL_DEPTH_FUNC, &value);
  depthFunc_ = static_cast<DepthFunc>(value);
  stencilTest_ = glIsEnabled(GL_STENCIL_TEST);
  scissorTest_ = glIsEnabled(GL_SCISSOR_TEST);
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxImageUnits_);
  textureUnits_.resize(maxImageUnits_, nullptr);
}

bool State::GetBlend() const { return blend_; }

bool State::GetDepthTest() const { return depthTest_; }

void State::SetDepthFunc(DepthFunc depthFunc) {
  if (depthFunc == depthFunc_) {
    return;
  }
  glDepthFunc(static_cast<GLenum>(depthFunc));
  depthFunc_ = depthFunc;
#ifdef DEBUG
  changes_++;
#endif
}

DepthFunc State::GetDepthFunc() const { return depthFunc_; }

void State::SetBlend(const bool blend) {
  if (blend == blend_) {
    return;
  }
  if (blend) {
    glEnable(GL_BLEND);
  } else {
    glDisable(GL_BLEND);
  }
  blend_ = blend;
#ifdef DEBUG
  changes_++;
#endif
}

void State::SetDepthTest(const bool depthTest) {
  if (depthTest == depthTest_) {
    return;
  }
  if (depthTest) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
  depthTest_ = depthTest;
#ifdef DEBUG
  changes_++;
#endif
}

bool State::IsStencilTest() const { return stencilTest_; }

void State::SetStencilTest(const bool stencilTest) {
  if (stencilTest == stencilTest_) {
    return;
  }
  if (stencilTest) {
    glEnable(GL_STENCIL_TEST);
  } else {
    glDisable(GL_STENCIL_TEST);
  }
  stencilTest_ = stencilTest;
#ifdef DEBUG
  changes_++;
#endif
}

bool State::IsScissorTest() const { return scissorTest_; }

void State::SetScissorTest(const bool scissorTest) {
  if (scissorTest == scissorTest_) {
    return;
  }
  if (scissorTest) {
    glEnable(GL_SCISSOR_TEST);
  } else {
    glDisable(GL_SCISSOR_TEST);
  }
  scissorTest_ = scissorTest;
#ifdef DEBUG
  changes_++;
#endif
}

void State::SetScissor(const Rect& rect) {
  if (scissor_ == rect) {
    return;
  }
  scissor_ = rect;
  glScissor(scissor_.LowerLeftPosition.x, scissor_.LowerLeftPosition.y,
            scissor_.Size.x, scissor_.Size.y);
#ifdef DEBUG
  changes_++;
#endif
}

Rect State::GetScissor() const { return scissor_; }

#ifdef DEBUG
void State::ResetChangeCounter() { changes_ = 0; }
#endif
void State::RegisterUbo(const int target, buffer::UniformBufferObject* ubo) {
  uboMap_[target] = ubo;
}

buffer::FrameBuffer* State::GetFramebuffer() const { return framebuffer_; }

void State::SetFramebuffer(buffer::FrameBuffer* const framebuffer) {
  if (framebuffer_ == framebuffer) {
    return;
  }
  framebuffer_ = framebuffer;
  if (framebuffer_ != nullptr) {
    framebuffer_->Bind();
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
#ifdef DEBUG
  changes_++;
#endif
}

Rect State::GetViewPort() const { return viewPort_; }

void State::SetViewPort(const Rect& rect) {
  if (viewPort_ == rect) {
    return;
  }
  viewPort_ = rect;
  glViewport(rect.LowerLeftPosition.x, rect.LowerLeftPosition.y, rect.Size.x,
             rect.Size.y);
#ifdef DEBUG
  changes_++;
#endif
}

char State::SetTexture(texture::Texture& texture) {
  if (texture.unit_ != -1) {
    return texture.unit_;
  }
  for (auto i = 1; i < textureUnits_.size(); ++i) {
    if (textureUnits_[i] == nullptr) {
      SetTexture(i, texture);
      return static_cast<char>(i);
    }
  }
  return -1;
}

void State::SetTexture(const byte textureUnit, texture::Texture& texture) {
  SetTexture(texture.GetType(), textureUnit, texture);
}

void State::SetTexture(const GLenum target, const byte textureUnit,
                       texture::Texture& texture) {
  if (textureUnit >= maxImageUnits_) {
    throw std::runtime_error(
        "State::SetTexture: textureUnit out of range, max. image units = " +
        std::to_string(maxImageUnits_));
  }
  if (textureUnits_[textureUnit] == &texture) {
    return;
  }
  if (textureUnits_[textureUnit] != nullptr) {
    textureUnits_[textureUnit]->unit_ = -1;
  }
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(target, texture.GetId());
  textureUnits_[textureUnit] = &texture;
  texture.unit_ = static_cast<char>(textureUnit);
#ifdef DEBUG
  changes_++;
#endif
}

int State::GetMaxImageUnits() const { return maxImageUnits_; }

void State::Clear(const BufferBitDescription bits) {
  GLbitfield bufferBits = 0;
  if (bits.Color) {
    bufferBits = bufferBits | GL_COLOR_BUFFER_BIT;
  }
  if (bits.Depth) {
    bufferBits = bufferBits | GL_DEPTH_BUFFER_BIT;
  }
  if (bits.Stencil) {
    bufferBits = bufferBits | GL_STENCIL_BUFFER_BIT;
  }
#ifdef DEBUG
  changes_++;
#endif
  glClear(bufferBits);
}

int State::GetChanges() const { return changes_; }

}  // namespace soil::video::render
