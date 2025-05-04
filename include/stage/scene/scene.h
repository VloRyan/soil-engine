
#ifndef ENGINE_STAGE_SCENE_SCENE_H
#define ENGINE_STAGE_SCENE_SCENE_H
#include "node.h"
#include "stage/manager.h"
#include "stage/resources.h"
#include "viewer/node.h"

namespace stage {
    class Stage;
}

namespace stage::scene {
    class Scene : public Node, public ::event::Handler<event::Node>, public ::event::Handler<event::Component>,
                  public input::EventHandler,
                  public engine::WindowEventHandler {
    public:
        explicit Scene(Stage *stage);

        void Update() override;

        void Handle(const event::Node &event) override;

        void Handle(const event::Component &event) override;

        void Handle(const input::Event &event) override;

        void Handle(const engine::WindowEvent &event) override;

        [[nodiscard]] const std::vector<viewer::Node *> &GetViewer() const;

        int GetNextId();

        void AddChild(Node *child) override;

        [[nodiscard]] Resources &GetResources() const;

        //void SetQuadTree(QuadTree *quadTree);
        virtual void Render(video::render::State &state);

        Stage* GetStage() const;

    protected:
        void RemoveChild(Node *node) override;

        virtual void ComponentStateChanged(component::Component *component);

        virtual void ComponentAdded(component::Component *component);

        virtual void ComponentRemoved(component::Component *component);

        virtual void NodeStateChanged(Node *node);

        virtual void NodeAdded(Node *node);

        virtual void NodeRemoved(const Node *node);

    private:
        static void forEachChild(const Node *node, const std::function<void(Node *child)> &func);

        [[nodiscard]] static Node* computeTopDirtyNode(Node *current);

        Stage *stage_;
        std::vector<viewer::Node *> viewer_;
        std::vector<Node *> activeUpdateNodes_;
        std::vector<Node *> dirtyNodes_;
        std::vector<Node *> dirtyNodes2_;
        std::vector<Node *> *dirtyNodesPtr_;
        std::vector<Node *> inputEventReceiverNodes_;
        std::vector<Node *> windowEventReceiverNodes_;
        int nextId_;
        glm::mat4 worldTransform_;
    };
}

#endif //ENGINE_STAGE_SCENE_SCENE_H
