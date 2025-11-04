#ifndef TEST_STAGE_SCENE_MOCKS
#define TEST_STAGE_SCENE_MOCKS

#include "glm/glm.hpp"
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

  void Reset() {
    Calls.Reset();
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

  void Handle(const WindowEvent& event) override {
    Calls.HandleWindowEvent++;
    Node::Handle(event);
  }

  void Handle(const input::Event& event) override {
    Calls.HandleInputEvent++;
    Node::Handle(event);
  }

  void SetReceiverType(const ReceiverType type,
                       const bool value = true) override {
    Node::SetReceiverType(type, value);
  }

  void SetUpdateType(const UpdateType type) override {
    Node::SetUpdateType(type);
  }
  struct {
    int Update{0};
    int UpdateDirty{0};
    int HandleComponentEvent{0};
    int HandleWindowEvent{0};
    int HandleInputEvent{0};
    void Reset() {
      Update = 0;
      UpdateDirty = 0;
      HandleInputEvent = 0;
      HandleWindowEvent = 0;
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
  int HandleComponentEventCalledCount = 0;

  void ResetMocks() { HandleComponentEventCalledCount = 0; }

  void Handle(const event::Component& event) override {
    HandleComponentEventCalledCount++;
    Scene::Handle(event);
  }
};
}  // namespace soil::stage::scene
#endif
