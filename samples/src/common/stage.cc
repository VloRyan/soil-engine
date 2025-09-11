#include "stage.h"

#include <utility>


#include "stage/scene/input.h"
#include "stage/scene/scene.h"

namespace soil_samples::common {
    Stage::Stage() : backAction_(nullptr) {}

    void Stage::Load() {
        OnLoad();
        if (GetScenes().size() != 1) {
            throw std::runtime_error("expected one scene after OnLoad()");
        }
        auto* scene = GetScenes()[0];
        auto* inputNode = scene->AddChild(new soil::stage::scene::Input());
        auto& inputMap = inputNode->GetEventMap();
        if (backAction_ != nullptr) {
            inputNode->GetEventMap(). //
                AddKeyMapping(soil::input::Keys::Escape, //
                              soil::input::Event::State::Release, //
                              [this](const soil::input::Event&) {
                                  backAction_(); //
                              });
        }
        RegisterInputEvents(inputMap);
        soil::stage::Stage::Load();
    }

    void Stage::SetBackAction(std::function<void()> backAction) {
        backAction_ = std::move(backAction);
    }
} // namespace soil_samples::common
