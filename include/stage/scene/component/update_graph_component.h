#ifndef SOIL_ENGINE_UPDATE_GRAPH_COMPONENT_H
#define SOIL_ENGINE_UPDATE_GRAPH_COMPONENT_H
#include "event_component.h"

namespace soil::stage::scene::component {
class UpdateGraphComponent : public EventComponent {
 public:
  explicit UpdateGraphComponent();
  ~UpdateGraphComponent() override = default;
  void OnEvent(const event::Node& event) override;
  void Update() override;

 private:
  void OnNodeStateChanged(Node* node);
  void OnNodeAdded(Node* node);
  void OnNodeRemoved(Node* node);
  static Node* computeTopDirtyNode(Node* node);
  std::vector<Node*> dirtyNodesFront_;
  std::vector<Node*> dirtyNodesBack_;
  std::vector<Node*>* dirtyNodesPtr_;
};
}  // namespace soil::stage::scene::component
#endif
