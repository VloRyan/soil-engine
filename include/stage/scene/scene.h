
#ifndef SOIL_STAGE_SCENE_SCENE_H
#define SOIL_STAGE_SCENE_SCENE_H
#include "node.h"
#include "stage/manager.h"
#include "stage/resources.h"
#include "viewer/node.h"

namespace soil::stage {
    class Stage;
}

namespace soil::stage::scene {
    class Scene : public Node,
                  public soil::event::Handler<event::Node>,
                  public soil::event::Handler<event::Component>,
                  public input::EventHandler,
                  public WindowEventHandler {
    public:
        explicit Scene(Stage *stage);

        void Update() override;

        void Handle(const event::Node &event) override;

        void Handle(const event::Component &event) override;

        void Handle(const input::Event &event) override;

        void Handle(const WindowEvent &event) override;

        [[nodiscard]] const std::vector<viewer::Node *> &GetViewer() const;

        int GetNextId();

        void AddChild(Node *child) override;

        [[nodiscard]] Resources &GetResources() const;

        virtual void Render(video::render::State &state);

        [[nodiscard]] Stage *GetStage() const;

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

        [[nodiscard]] static Node *computeTopDirtyNode(Node *current);

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
} // namespace soil::stage::scene

#endif // SOIL_STAGE_SCENE_SCENE_H
