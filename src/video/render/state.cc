
#include "video/render/state.h"

#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <plog/Log.h>

#include "video/glfw_window.h"
#include "video/shader/shader.h"

namespace soil::video::render {
State::State(Context& context)
    : context_(context),
      depthTest_(false),
      depthFunc_(DepthFunc::Less),
      stencilTest_(false),
      scissorTest_(false),
      blend_(false),
      changes_(0),
      maxImageUnits_(0),
      uboMatrices_(nullptr),
      framebuffer_(nullptr),
      viewPort_({}),
      scissor_({}),
      vao_(nullptr),
      clearColor_(),
      shaderProgramId_(0) {
  blend_ = context_.IsEnabled(GL_BLEND);
  depthTest_ = context_.IsEnabled(GL_DEPTH_TEST);
  depthFunc_ = static_cast<DepthFunc>(context_.GetInteger(GL_DEPTH_FUNC));
  stencilTest_ = context_.IsEnabled(GL_STENCIL_TEST);
  scissorTest_ = context_.IsEnabled(GL_SCISSOR_TEST);
  maxImageUnits_ = context_.GetInteger(GL_MAX_TEXTURE_IMAGE_UNITS);
  textureUnits_.resize(maxImageUnits_, nullptr);
}

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
  if (def.ScissorTest.has_value()) {
    this->SetScissorTest(def.ScissorTest.value());
  }
  if (def.DepthFunc.has_value()) {
    this->SetDepthFunc(def.DepthFunc.value());
  }
  if (def.ClearColor.has_value()) {
    this->SetClearColor(def.ClearColor.value());
  }
  if (def.Framebuffer.has_value()) {
    this->SetFramebuffer(def.Framebuffer.value());
  }
  if (def.ViewPort.has_value()) {
    this->SetViewPort(def.ViewPort.value());
  }
  if (def.Scissor.has_value()) {
    this->SetScissor(def.Scissor.value());
  }
}

void State::WriteUbo(
    const int target,
    const std::function<void(buffer::Cursor& cursor)>& writeFunc) {
  const auto itr = uboMap_.find(target);
  if (itr == uboMap_.end()) {
    throw std::runtime_error("Cannot find Ubo for target: " +
                             std::to_string(target));
  }
  auto& cursor = itr->second->GetCursor();
  writeFunc(cursor);
  itr->second->Flush();
#ifdef DEBUG
  changes_++;
#endif
}

bool State::GetBlend() const { return blend_; }

bool State::GetDepthTest() const { return depthTest_; }

void State::SetDepthFunc(DepthFunc depthFunc) {
  if (depthFunc == depthFunc_) {
    return;
  }
  context_.SetDepthFunc(depthFunc);
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
    context_.Enable(GL_BLEND);
  } else {
    context_.Disable(GL_BLEND);
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
    context_.Enable(GL_DEPTH_TEST);
  } else {
    context_.Disable(GL_DEPTH_TEST);
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
    context_.Enable(GL_STENCIL_TEST);
  } else {
    context_.Disable(GL_STENCIL_TEST);
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
    context_.Enable(GL_SCISSOR_TEST);
  } else {
    context_.Disable(GL_SCISSOR_TEST);
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
    context_.BindFramebuffer(GL_FRAMEBUFFER, 0);
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
  context_.Viewport(rect.LowerLeftPosition.x, rect.LowerLeftPosition.y,
                    rect.Size.x, rect.Size.y);
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
  context_.ActiveTexture(GL_TEXTURE0 + textureUnit);
  context_.BindTexture(target, texture.GetId());
  textureUnits_[textureUnit] = &texture;
  texture.unit_ = static_cast<char>(textureUnit);
#ifdef DEBUG
  changes_++;
#endif
}

int State::GetChanges() const { return changes_; }

texture::Texture* State::GetTexture(byte textureUnit) {
  if (textureUnit >= maxImageUnits_) {
    return nullptr;
  }
  return textureUnits_.at(textureUnit);
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
  context_.Clear(bufferBits);
}
const glm::vec4& State::GetClearColor() const { return clearColor_; }
void State::SetClearColor(const glm::vec4& clearColor) {
  if (clearColor_ == clearColor) {
    return;
  }
  clearColor_ = clearColor;
#ifdef DEBUG
  changes_++;
#endif
  context_.ClearColor(clearColor_.r, clearColor_.g, clearColor_.b,
                      clearColor_.a);
}

StateDef State::Pop() {
  return {
      .Blend = blend_,
      .DepthTest = depthTest_,
      .StencilTest = stencilTest_,
      .ScissorTest = scissorTest_,
      .DepthFunc = depthFunc_,
      .ClearColor = clearColor_,
      .Framebuffer = framebuffer_,
      .ViewPort = viewPort_,
      .Scissor = scissor_,
  };
}
void State::BindVao(const vertex::Vao* vao) {
  if (vao == vao_) {
    return;
  }
#ifdef DEBUG
  changes_++;
#endif
  if (vao != nullptr) {
    context_.BindVertexArray(vao->GetId());
  } else {
    context_.BindVertexArray(0);
  }
  vao_ = vao;
}
const vertex::Vao* State::GetVao() const { return vao_; }

void State::SetShader(const shader::Shader* shader) {
  if (shader == nullptr) {
    if (shaderProgramId_ == 0) {
      return;
    }
#ifdef DEBUG
    changes_++;
#endif
    context_.UseProgram(0);
    return;
  }
  if (shaderProgramId_ == shader->GetId()) {
    return;
  }
#ifdef DEBUG
  changes_++;
#endif
  context_.UseProgram(shader->GetId());
}

bool StateDef::operator==(const StateDef& rhs) const {
  return Blend == rhs.Blend && DepthTest == rhs.DepthTest &&
         StencilTest == rhs.StencilTest && ScissorTest == rhs.ScissorTest &&
         DepthFunc == rhs.DepthFunc;
}
bool StateDef::operator!=(const StateDef& rhs) const { return !(rhs == *this); }
bool StateDef::operator<(const StateDef& rhs) const {
  if (Blend < rhs.Blend) return true;
  if (rhs.Blend < Blend) return false;
  if (DepthTest < rhs.DepthTest) return true;
  if (rhs.DepthTest < DepthTest) return false;
  if (StencilTest < rhs.StencilTest) return true;
  if (rhs.StencilTest < StencilTest) return false;
  if (ScissorTest < rhs.ScissorTest) return true;
  if (rhs.ScissorTest < ScissorTest) return false;
  return DepthFunc < rhs.DepthFunc;
}
bool StateDef::operator>(const StateDef& rhs) const { return rhs < *this; }
bool StateDef::operator<=(const StateDef& rhs) const { return !(rhs < *this); }
bool StateDef::operator>=(const StateDef& rhs) const { return !(*this < rhs); }
}  // namespace soil::video::render
