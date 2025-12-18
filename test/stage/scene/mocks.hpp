#ifndef TEST_STAGE_SCENE_MOCKS
#define TEST_STAGE_SCENE_MOCKS

#include "glm/glm.hpp"
#include "stage/hook/event_hook.hpp"
#include "stage/scene/node.h"
#include "stage/scene/scene.h"

namespace soil::stage::scene {
class NodeMock : public Node {
 public:
  explicit NodeMock(const Type type = Type::Transform) : Node(type) {}

  ~NodeMock() override = default;
  std::vector<Node*> AddedChildren;
  std::vector<Node*> RemovedChildren;
  std::bitset<4> UpdateDirtyImpacts;
  std::function<void()> UpdateFunc = nullptr;
  hook::EventHook<stage::event::Node>* NodeEventHook = nullptr;
  class Stage* StageOverride = nullptr;
  void Reset() {
    Calls = Calls_t{};
    AddedChildren.clear();
    RemovedChildren.clear();
    UpdateDirtyImpacts = 0;
    UpdateFunc = nullptr;
  }

  void Update() override {
    Calls.Update++;
    if (UpdateFunc != nullptr) {
      UpdateFunc();
    }
    Node::Update();
  }

  void UpdateDirty() override {
    Calls.UpdateDirty++;
    UpdateDirtyImpacts = GetDirtyImpacts();
    Node::UpdateDirty();
  }

  void addChild(Node* node) override {
    Node::addChild(node);
    AddedChildren.push_back(node);
  }

  void RemoveChild(Node* node) override {
    Node::RemoveChild(node);
    RemovedChildren.push_back(node);
  }

  void Handle(const event::Component& event) override {
    Calls.HandleComponentEvent++;
    Node::Handle(event);
  }

  void SetReceiverType(const ReceiverType type,
                       const bool value = true) override {
    Node::SetReceiverType(type, value);
  }
  
  class Stage* Stage() const override {
    if (StageOverride != nullptr) {
      return StageOverride;
    }
    return Node::Stage();
  }

 protected:
  void fire(const event::Node& event) const override {
    Node::fire(event);
    if (NodeEventHook != nullptr) {
      NodeEventHook->OnEvent(event);
    }
  }

 public:
  struct Calls_t {
    int Update{0};
    int UpdateDirty{0};
    int HandleComponentEvent{0};
    void Reset() {
      Update = 0;
      UpdateDirty = 0;
      HandleComponentEvent = 0;
    }
  } Calls;
};

class NodeEventMockListener : public event::NodeEventHandler {
 public:
  std::vector<event::Node> Events;

  void ResetMocks() { Events.clear(); }

  void Handle(const event::Node& event) override { Events.push_back(event); }
};

class SceneMock : public Scene {
 public:
  int HandleComponentEventCalledCount = 0;

  void ResetMocks() { HandleComponentEventCalledCount = 0; }

  void Handle(const event::Component& event) override {
    HandleComponentEventCalledCount++;
    Scene::Handle(event);
  }
  void SetStage(class Stage* stage) override { Scene::SetStage(stage); }
};
}  // namespace soil::stage::scene
#endif
