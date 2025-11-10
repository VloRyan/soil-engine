
#include "stage/scene/scene.h"

#include <deque>

#include "stage/scene/component/render/render_state_container_component.h"
#include "stage/scene/component/update_graph_component.h"
#include "stage/scene/node.h"
#include "stage/stage.h"

namespace soil::stage::scene {
Scene::Scene() : Node(Type::Scene), stage_(nullptr) {
  AddComponent(new component::UpdateGraphComponent());
  AddComponent(new component::render::RenderStateContainerComponent());
  incorporateAddedComponents();
}

Scene::~Scene() {
  if (const auto stage = GetStage(); stage != nullptr) {
    stage->RemoveScene(this);
  }
}

void Scene::SetStage(Stage* stage) {
  if (stage == stage_) {
    return;
  }
  if (stage_ != nullptr) {
    stage_->RemoveScene(this);
  }
  auto prevStage = stage_;
  stage_ = stage;
  OnStageChanged(stage_, prevStage);
}

Stage* Scene::GetStage() const { return stage_; }
}  // namespace soil::stage::scene
