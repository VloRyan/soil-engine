
#include "stage/stage.h"

#include <GL/gl3w.h>
#include <stdexcept>

#include "stage/scene/scene.h"

namespace stage
{
    Stage::Stage(Manager *manager) :
        id_(manager->AddStage(this)), resources_(manager), nextSceneId_(0), frameBuffer_(nullptr), quad_(nullptr)
    {
    }

    Stage::~Stage()
    {
        for (const auto *scene : scenes_)
        {
            delete scene;
        }
        delete frameBuffer_;
    }

    void Stage::Update()
    {
        for (auto *scene : scenes_)
        {
            scene->Update();
        }
    }

    void Stage::Render(video::render::State &state)
    {
        state.SetFramebuffer(frameBuffer_);
        if (frameBuffer_ != nullptr)
        {
            // state.SetViewPort(frameBuffer_->GetSize());
            glViewport(0, 0, frameBuffer_->GetSize().x, frameBuffer_->GetSize().y);
        }
        for (auto *scene : scenes_)
        {
            scene->Render(state);
        }
        if (frameBuffer_ != nullptr)
        {
            state.SetFramebuffer(nullptr);
            state.SetViewPort(GetResources().GetWindow()->GetSize());
            quad_->SetTexture(frameBuffer_->GetColorBufferTexture(0));
            quad_->Render();
        }
    }

    int Stage::AddScene(scene::Scene *scene)
    {
        // static_assert(std::is_base_of_v<scene::Scene, std::remove_pointer_t<T> >,
        //             "type parameter of this class must derive from Scene");
        // scene->Init(resources_);
        scenes_.emplace_back(scene);
        return ++nextSceneId_;
    }

    void Stage::Handle(const input::Event &event)
    {
        for (auto *scene : scenes_)
        {
            scene->Handle(event);
        }
    }

    void Stage::Handle(const engine::WindowEvent &event)
    {
        if (event.GetCause() == engine::WindowEvent::SizeChanged)
        {
            if (frameBuffer_ != nullptr)
            {
                frameBuffer_->Resize(event.GetWindow()->GetRenderSize());
            }
        }
        for (auto *scene : scenes_)
        {
            scene->Handle(event);
        }
    }

    Resources &Stage::GetResources() const { return *resources_; }

    video::buffer::FrameBuffer *Stage::GetFrameBuffer() const { return frameBuffer_; }

    void Stage::SetFrameBuffer(video::buffer::FrameBuffer *const frameBuffer, video::render::Quad *quad)
    {
        frameBuffer_ = frameBuffer;
        quad_ = quad;
#ifdef DEBUG
        if (frameBuffer_ != nullptr && quad_ == nullptr)
        {
            throw std::runtime_error("setting framebuffer without quad is not permitted");
        }
#endif
    }

    video::render::Quad *Stage::GetQuad() const { return quad_; }
} // namespace stage
