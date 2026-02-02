#ifndef SOIL_ENGINE_STAGE_SCENE_COMPONENT_UPDATE_GRAPH_COMPONENT_H
#define SOIL_ENGINE_STAGE_SCENE_COMPONENT_UPDATE_GRAPH_COMPONENT_H
#include "stage/scene/component/event/event_component.h"

namespace soil::stage::scene::component {
class UpdateGraphComponent : public event::EventComponent {
 public:
  explicit UpdateGraphComponent();
  ~UpdateGraphComponent() override = default;
  void OnEvent(const stage::event::Node& event) override;
  void Update() override;
  void OnTrigger(const TriggerPoint& point) override;

  const std::vector<Node*>& DirtyNodes();
  const std::vector<Node*>& NodesToDelete();

 private:

  void onNodeStateChanged(Node* node);
  void onNodeAdded(Node* node);
  void onNodeRemoved(Node* node);
  static Node* computeTopDirtyNode(Node* node);
  static bool removeNode(Node* node, std::vector<Node*>& vector);
  std::vector<Node*> dirtyNodesFront_;
  std::vector<Node*> dirtyNodesBack_;
  std::vector<Node*>* dirtyNodesPtr_;
  std::vector<Node*> nodesToDelete_;
  bool deletingNodes_;
};
}  // namespace soil::stage::scene::component
#endif
