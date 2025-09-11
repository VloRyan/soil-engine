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
        int ComputeWorldTransformCalledCount = 0;
        int UpdateCalledCount = 0;
        int UpdateDirtyCalledCount = 0;
        int HandleComponentEventCalledCount = 0;
        int HandleWindowEventCalledCount = 0;
        int HandleInputEventCalledCount = 0;
        std::vector<Node*> AddedChildren;
        std::vector<Node*> RemovedChildren;
        std::bitset<4> UpdateDirtyImpacts;
        std::function<void()> UpdateFunc = nullptr;

        void ResetMocks() {
            ComputeWorldTransformCalledCount = 0;
            UpdateCalledCount = 0;
            UpdateDirtyCalledCount = 0;
            HandleComponentEventCalledCount = 0;
            UpdateFunc = nullptr;
        }

        void Update() override {
            UpdateCalledCount++;
            if (UpdateFunc != nullptr) {
                UpdateFunc();
            }
            Node::Update();
        }

        void UpdateDirty() override {
            UpdateDirtyImpacts = GetDirtyImpacts();
            UpdateDirtyCalledCount++;
            Node::UpdateDirty();
        }

        void ComputeWorldTransform(const glm::mat4& parentWorldTransform) override {
            ComputeWorldTransformCalledCount++;
            Node::ComputeWorldTransform(parentWorldTransform);
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
            HandleComponentEventCalledCount++;
            Node::Handle(event);
        }

        void Handle(const WindowEvent& event) override {
            HandleWindowEventCalledCount++;
            Node::Handle(event);
        }

        void Handle(const input::Event& event) override {
            HandleInputEventCalledCount++;
            Node::Handle(event);
        }

        void SetReceiverType(const ReceiverType type, const bool value = true) override {
            Node::SetReceiverType(type, value);
        }

        void SetUpdateType(const UpdateType type) override {
            Node::SetUpdateType(type);
        }
    };

    class NodeEventMockListener : public event::NodeEventHandler {
    public:
        std::vector<event::Node> Events;

        void ResetMocks() {
            Events.clear();
        }

        void Handle(const event::Node& event) override {
            Events.push_back(event);
        }
    };

    class SceneMock : public Scene {
        int HandleComponentEventCalledCount = 0;

        void ResetMocks() {
            HandleComponentEventCalledCount = 0;
        }

        void Handle(const event::Component& event) override {
            HandleComponentEventCalledCount++;
            Scene::Handle(event);
        }
    };
} // namespace soil::stage::scene
#endif
