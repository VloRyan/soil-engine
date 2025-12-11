#ifndef SOIL_ENGINE_UPDATE_MATRICES_UBO_COMPONENT_H
#define SOIL_ENGINE_UPDATE_MATRICES_UBO_COMPONENT_H
#include "stage/event/node.h"
#include "stage/scene/component/event/event_component.h"
#include "stage/scene/viewer/node.h"
#include "video/render/state.h"
namespace soil::stage::scene::component::render {
class UpdateMatricesUboComponent : public event::EventComponent {
 public:
  UpdateMatricesUboComponent(viewer::Node* viewer, int uboMatricesBindingTarget,
                             video::render::State* renderState);
  ~UpdateMatricesUboComponent() override = default;

 protected:
  void OnTrigger(const hook::TriggerHook::TriggerPoint& point) override;
  void SetParent(soil::stage::scene::Node* parent) override;

 private:
  int uboMatricesBindingTarget_;
  viewer::Node* viewer_;
  video::render::State* renderState_;
};
}  // namespace soil::stage::scene::component::render

#endif
