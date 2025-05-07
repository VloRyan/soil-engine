
#include "stage/stage.h"
#include "stage/scene/scene.h"

namespace soil::stage {
    Stage::Stage(Manager *manager) :
        id_(manager->AddStage(this)), resources_(&manager->GetResources()), nextSceneId_(0), frameBuffer_(nullptr),
        quad_(nullptr) {}

    Stage::~Stage() {
        for (const auto *scene : scenes_) {
            delete scene;
        }
        delete frameBuffer_;
    }

    void Stage::Update() {
        for (auto *scene : scenes_) {
            scene->Update();
        }
    }

    void Stage::Render(video::render::State &state) {
        state.SetFramebuffer(frameBuffer_);
        if (frameBuffer_ != nullptr) {
            state.SetViewPort(frameBuffer_->GetSize());
        } else {
            state.SetViewPort(GetResources().GetWindow()->GetSize());
        }
        for (auto *scene : scenes_) {
            scene->Render(state);
        }
        if (frameBuffer_ != nullptr) {
            state.SetFramebuffer(nullptr);
            state.SetViewPort(GetResources().GetWindow()->GetSize());
            quad_->SetTexture(frameBuffer_->GetColorBufferTexture(0));
            quad_->Render();
        }
    }

    int Stage::AddScene(scene::Scene *scene) {
        scenes_.emplace_back(scene);
        return ++nextSceneId_;
    }

    void Stage::Handle(const input::Event &event) {
        for (auto *scene : scenes_) {
            scene->Handle(event);
        }
    }

    void Stage::Handle(const WindowEvent &event) {
        if (event.GetCause() == WindowEvent::SizeChanged) {
            if (frameBuffer_ != nullptr) {
                frameBuffer_->Resize(event.GetWindow()->GetRenderSize());
            }
        }
        for (auto *scene : scenes_) {
            scene->Handle(event);
        }
    }

    Resources &Stage::GetResources() const { return *resources_; }

    video::buffer::FrameBuffer *Stage::GetFrameBuffer() const { return frameBuffer_; }

    void Stage::SetFrameBuffer(video::buffer::FrameBuffer *const frameBuffer, video::render::Quad *quad) {
        frameBuffer_ = frameBuffer;
        quad_ = quad;
#ifdef DEBUG
        if (frameBuffer_ != nullptr && quad_ == nullptr) {
            throw std::runtime_error("setting framebuffer without quad is not permitted");
        }
#endif
    }

    video::render::Quad *Stage::GetQuad() const { return quad_; }
} // namespace soil::stage
