
#include "stage/stage.h"

#include "stage/scene/scene.h"

namespace soil::stage {
    Stage::Stage() : loaded_(false), resources_(nullptr) {}

    Stage::~Stage() {
        if (IsLoaded()) {
            Stage::Unload();
        }
        for (auto* scene : scenes_) {
            scene->stage_ = nullptr;
            delete scene;
        }
    }

    void Stage::Update() {
        for (auto* scene : scenes_) {
            scene->Update();
        }
    }

    void Stage::Render(video::render::State& state) {
        for (auto* scene : scenes_) {
            scene->Render(state);
        }
    }

    void Stage::addScene(scene::Scene* scene) {
        scene->SetStage(this);
        scenes_.emplace_back(scene);
    }

    std::vector<scene::Scene*> Stage::GetScenes() const {
        return scenes_;
    }

    void Stage::Handle(const input::Event& event) {
        for (auto* scene : scenes_) {
            scene->Handle(event);
        }
    }

    void Stage::Handle(const WindowEvent& event) {
        for (auto* scene : scenes_) {
            scene->Handle(event);
        }
    }

    void Stage::RemoveScene(const scene::Scene* scene) {
        for (auto itr = scenes_.begin(); itr != scenes_.end(); ++itr) {
            if (*itr == scene) {
                scenes_.erase(itr);
                break;
            }
        }
    }

    Resources& Stage::GetResources() const {
        if (resources_ == nullptr) {
            throw StageNotRegisteredException();
        }
        return *resources_;
    }

    void Stage::Load() {
        loaded_ = true;
    }

    void Stage::Unload() {
        loaded_ = false;
    }

    bool Stage::IsLoaded() const {
        return loaded_;
    }
} // namespace soil::stage
