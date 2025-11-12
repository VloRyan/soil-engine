
#include "stage/scene/scene.h"

#include "stage/scene/component/render/render_state_container_component.h"
#include "stage/scene/component/update_graph_component.h"
#include "stage/scene/node.h"
#include "stage/stage.h"

namespace soil::stage::scene {
Scene::Scene() : Node(Type::Scene), stage_(nullptr) {
  AddComponent(new component::UpdateGraphComponent());
  AddComponent(new component::render::RenderStateContainerComponent(
      new video::render::StateContainer()));
  incorporateAddedComponents();
}

Scene::~Scene() {
  if (stage_ != nullptr) {
    stage_->RemoveScene(this);
  }
}

void Scene::SetStage(class Stage* stage) {
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

Stage* Scene::Stage() const { return stage_; }
}  // namespace soil::stage::scene
