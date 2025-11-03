
#include "stage/scene/scene.h"

#include <deque>

#include "stage/scene/component/update_graph_component.h"
#include "stage/scene/node.h"
#include "stage/stage.h"

namespace soil::stage::scene {
Scene::Scene()
    : Node(Type::Scene),
      stage_(nullptr),
      renderContainer_(new video::render::Container()),
      pipeline_(nullptr) {
  AddComponent(new component::UpdateGraphComponent());
  Node::Update();  // add components
}

Scene::~Scene() {
  if (const auto stage = GetStage(); stage != nullptr) {
    stage->RemoveScene(this);
  }
  delete renderContainer_;
}

void Scene::Render(video::render::State& state) {
  if (pipeline_ != nullptr) {
    pipeline_->Run(state);
  }
}

void Scene::Update() {
  for (const auto* node : nodesToDelete_) {
    delete node;
  }
  nodesToDelete_.clear();
  ForEachComponent(
      [](component::Component* component) { component->Update(); });
}

video::render::Container* Scene::GetRenderContainer() const {
  return renderContainer_;
}

video::render::Pipeline* Scene::GetPipeline() const { return pipeline_; }

void Scene::SetPipeline(video::render::Pipeline* const pipeline) {
  pipeline_ = pipeline;
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
