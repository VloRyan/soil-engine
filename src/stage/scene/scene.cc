
#include "stage/scene/scene.h"

#include "stage/scene/component/render/drawable_container_component.h"
#include "stage/scene/component/update_graph_component.h"
#include "stage/scene/node.h"
#include "stage/stage.h"
#include "video/render/forward/forward_rendering.h"

namespace soil::stage::scene {
Scene::Scene() : Scene(nullptr) {}
Scene::Scene(const video::render::Algorythm* algorythm)
    : Node(Type::Scene), stage_(nullptr), renderAlgorythm_(algorythm) {
  AddComponent(new component::UpdateGraphComponent());
  drawableContainer_ =
      AddComponent(new component::render::DrawableContainerComponent());
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
  if (renderAlgorythm_ == nullptr) {
    renderAlgorythm_ =
        video::Manager::GetRenderAlgorithm();  // set default algorithm
  }
  OnStageChanged(stage_, prevStage);
}

Stage* Scene::Stage() const { return stage_; }

void Scene::Render(video::render::State& state) {
  if (renderAlgorythm_ == nullptr) {
    return;
  }
  renderAlgorythm_->Render(state, drawableContainer_->Container());
}

}  // namespace soil::stage::scene
