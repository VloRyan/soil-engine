
#include "stage/scene/scene.h"

#include "stage/scene/component/render/drawable_container_component.h"
#include "stage/scene/component/update_graph_component.h"
#include "stage/scene/node.h"
#include "stage/stage.h"
#include "video/render/forward/forward_rendering.h"

namespace soil::stage::scene {
Scene::Scene() : Node(Type::Scene), stage_(nullptr) {
  video::render::Algorythm* rendering =
      new video::render::forward::ForwardRendering();
  AddComponent(new component::UpdateGraphComponent());
  AddComponent(new component::render::DrawableContainerComponent(rendering));
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
