
#ifndef SOIL_STAGE_SCENE_SCENE_H
#define SOIL_STAGE_SCENE_SCENE_H
#include "node.h"
#include "render/instancing.h"
#include "stage/manager.h"
#include "stage/resources.h"
#include "video/render/container.h"
#include "video/render/pipeline.h"
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

        int GetNextId();

        void AddChild(Node *child) override;

        [[nodiscard]] Resources &GetResources() const;

        virtual void Render(video::render::State &state);

        [[nodiscard]] Stage *GetStage() const;

        [[nodiscard]] virtual int GetUboMatricesBindingTarget() const;
        virtual void SetUboMatricesBindingTarget(int ubo_matrices_binding_target);
        [[nodiscard]] virtual video::render::Pipeline *GetPipeline() const;
        virtual void SetPipeline(video::render::Pipeline *pipeline);
        virtual void SetBeforeRenderViewer(
            const std::function<void(viewer::Node *node, video::render::Pipeline *pipeline)> &before_render_viewer);

        [[nodiscard]] viewer::Node *GetViewer() const;

        template <class T>
        T AddComponentFeature(T feature) {
            using type = std::remove_pointer_t<T>;
            static_assert(std::is_base_of_v<ComponentFeature, type>, "comp must be derived from ComponentFeature");
            componentFeatures_.push_back(feature);
            feature->OnAddedToScene(this);
            return feature;
        }

        virtual void RemoveComponentFeature(ComponentFeature *feature);
        [[nodiscard]] virtual video::render::Container *GetRenderContainer() const;

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
        viewer::Node *viewer_;
        std::vector<Node *> activeUpdateNodes_;
        std::vector<Node *> dirtyNodes_;
        std::vector<Node *> dirtyNodes2_;
        std::vector<Node *> *dirtyNodesPtr_;
        std::vector<Node *> inputEventReceiverNodes_;
        std::vector<Node *> windowEventReceiverNodes_;
        std::vector<ComponentFeature *> componentFeatures_;
        video::render::Container *renderContainer_;
        int nextId_;
        glm::mat4 worldTransform_;
        int uboMatricesBindingTarget_;
        video::render::Pipeline *pipeline_;
        std::function<void(viewer::Node *node, video::render::Pipeline *pipeline)> beforeRenderViewer_;
    };
} // namespace soil::stage::scene

#endif // SOIL_STAGE_SCENE_SCENE_H
