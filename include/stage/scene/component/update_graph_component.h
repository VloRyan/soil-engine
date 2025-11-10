#ifndef SOIL_ENGINE_UPDATE_GRAPH_COMPONENT_H
#define SOIL_ENGINE_UPDATE_GRAPH_COMPONENT_H
#include "stage/scene/component/event/event_component.h"

namespace soil::stage::scene::component {
class UpdateGraphComponent : public event::EventComponent {
 public:
  explicit UpdateGraphComponent();
  ~UpdateGraphComponent() override = default;
  void OnEvent(const stage::event::Node& event) override;
  void Update() override;
  void OnTrigger(const TriggerPoint& point) override;

 private:
  void OnNodeStateChanged(Node* node);
  void OnNodeAdded(Node* node);
  void OnNodeRemoved(Node* node);
  static Node* computeTopDirtyNode(Node* node);
  std::vector<Node*> dirtyNodesFront_;
  std::vector<Node*> dirtyNodesBack_;
  std::vector<Node*>* dirtyNodesPtr_;
  std::vector<Node*> nodesToDelete_;
};
}  // namespace soil::stage::scene::component
#endif
