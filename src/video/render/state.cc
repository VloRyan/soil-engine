
#include "video/render/state.h"

#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <plog/Log.h>

#include "video/mesh/mesh.h"
#include "window.h"

namespace video::render
{
    State::State() : blend_(false), depthTest_(false), stencilTest_(false), changes_(0) {}

    void State::Apply(const StateDef &def)
    {
        this->SetBlend(def.Blend);
        this->SetDepthTest(def.DepthTest);
        this->SetStencilTest(def.StencilTest);
    }

    void State::WriteUbo(const int target, const std::function<void(buffer::Cursor *cursor)> &writeFunc)
    {
        const auto itr = uboMap_.find(target);
        if (itr == uboMap_.end())
        {
            throw std::runtime_error("Cannot find Ubo for target: " + std::to_string(target));
        }
        auto *cursor = itr->second->GetCursor();
        writeFunc(cursor);
        itr->second->Flush();
#ifdef DEBUG
        changes_++;
#endif
    }

    void State::Init()
    {
        blend_ = glIsEnabled(GL_BLEND);
        depthTest_ = glIsEnabled(GL_DEPTH_TEST);
        stencilTest_ = glIsEnabled(GL_STENCIL_TEST);
    }

    bool State::GetBlend() const { return blend_; }

    bool State::GetDepthTest() const { return depthTest_; }

    void State::SetBlend(const bool blend)
    {
        if (blend == blend_)
        {
            return;
        }
        if (blend)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }
        blend_ = blend;
#ifdef DEBUG
        changes_++;
#endif
    }

    void State::SetDepthTest(const bool depthTest)
    {
        if (depthTest == depthTest_)
        {
            return;
        }
        if (depthTest)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
        depthTest_ = depthTest;
#ifdef DEBUG
        changes_++;
#endif
    }

    bool State::IsStencilTest() const { return stencilTest_; }

    void State::SetStencilTest(const bool stencilTest)
    {
        stencilTest_ = stencilTest;
        if (stencilTest == stencilTest_)
        {
            return;
        }
        if (stencilTest)
        {
            glEnable(GL_STENCIL_TEST);
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }
        stencilTest_ = stencilTest;
#ifdef DEBUG
        changes_++;
#endif
    }
#ifdef DEBUG
    void State::ResetChangeCounter() { changes_ = 0; }
#endif
    void State::RegisterUbo(const int target, buffer::UniformBufferObject *ubo) { uboMap_[target] = ubo; }

    buffer::FrameBuffer *State::GetFramebuffer() const { return framebuffer_; }

    void State::SetFramebuffer(buffer::FrameBuffer *const framebuffer)
    {
        if (framebuffer_ == framebuffer)
        {
            return;
        }
        framebuffer_ = framebuffer;
        if (framebuffer_ != nullptr)
        {
            framebuffer_->Bind();
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
#ifdef DEBUG
        changes_++;
#endif
    }

    glm::vec2 State::GetViewPort() const { return viewPort_; }

    void State::SetViewPort(const glm::vec2 &viewPort)
    {
        if (viewPort_ == viewPort)
        {
            return;
        }
        viewPort_ = viewPort;
        glViewport(0, 0, viewPort_.x, viewPort_.y);
#ifdef DEBUG
        changes_++;
#endif
    }
} // namespace video::render
