#include "stage.h"

#include <plog/Log.h>

#include <utility>

#include "stage/scene/component/input/input_component.h"
#include "stage/scene/scene.h"
namespace soil_samples::common {
Stage::Stage() : backAction_(nullptr), printStatistics_(false) {}

void Stage::Load() {
  OnLoad();
  if (GetScenes().size() != 1) {
    throw std::runtime_error("expected one scene after OnLoad()");
  }
  auto* scene = GetScenes()[0];
  auto* inputComp = scene->AddComponent(
      new soil::stage::scene::component::input::InputComponent());
  auto& inputMap = inputComp->EventMap();
  if (backAction_ != nullptr) {
    inputMap.                                                  //
        AddKeyMapping(soil::input::Keys::Escape,               //
                      soil::input::Event::StateType::Release,  //
                      [this](const soil::input::Event&) {
                        backAction_();  //
                      });
  }
  RegisterInputEvents(inputMap);
  soil::stage::Stage::Load();
}

void Stage::SetBackAction(std::function<void()> backAction) {
  backAction_ = std::move(backAction);
}

void Stage::Handle(const soil::event::EngineEvent& event) {
  soil::stage::Stage::Handle(event);
  if (event.Cause == soil::event::EngineEvent::CauseType::StatisticsChanged) {
    const auto stats = event.Engine->GetStatistics();
    OnStatsChanges(stats);
  }
}

void Stage::OnStatsChanges(const soil::Engine::Statistics& stats) {
  if (printStatistics_) {
    PLOG_DEBUG << "FPS: " << std::to_string(stats.FPS)
               << " Draws: " << std::to_string(stats.DrawCount / stats.FPS)
               << " Vertices: " << std::to_string(stats.VertexCount / stats.FPS)
               << " State changes: "
               << std::to_string(stats.StateChanges / stats.FPS)
               << " Update times: "
               << std::to_string(stats.updateInputTime / stats.FPS) << ", "
               << std::to_string(stats.updateStageTime / stats.FPS) << ", "
               << std::to_string(stats.updateVideoTime / stats.FPS)
               << " Render times: "
               << std::to_string(stats.startRenderTime / stats.FPS) << ", "
               << std::to_string(stats.renderTime / stats.FPS) << ", "
               << std::to_string(stats.endRenderTime / stats.FPS);
  }
}
}  // namespace soil_samples::common
