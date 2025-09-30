
#ifndef SOIL_STAGE_SCENE_SCENE_H
#define SOIL_STAGE_SCENE_SCENE_H
#include "hook/hook.h"
#include "hook/update_hook.h"
#include "node.h"
#include "render/instancing.h"
#include "stage/manager.h"
#include "stage/resources.h"
#include "stage/event/game_event.h"
#include "video/render/container.h"
#include "video/render/pipeline.h"

namespace soil::stage {
    class Stage;
}

namespace soil::stage::scene {
    class Scene : public Node,
                  public soil::event::Handler<event::Node>,
                  public soil::event::Handler<event::Component>,
                  public input::EventHandler,
                  public WindowEventHandler,
                  public event::GameEventHandler {
    public:
        friend class soil::stage::Stage;

        explicit Scene();

        ~Scene() override;

        void Update() override;

        void Handle(const event::Node& event) override;

        void Handle(const event::Component& event) override;

        void Handle(const input::Event& event) override;

        void Handle(const WindowEvent& event) override;

        void Handle(const event::GameEvent& event) override;

        virtual void Render(video::render::State& state);

        [[nodiscard]] Stage* GetStage() const;

        [[nodiscard]] virtual video::render::Pipeline* GetPipeline() const;

        virtual void SetPipeline(video::render::Pipeline* pipeline);

        template <class T>
        T AddHook(T theHook) {
            using type = std::remove_pointer_t<T>;
            static_assert(std::is_base_of_v<hook::Hook, type>, "hook must be derived from hook::Hook");
            addHook(theHook);
            return theHook;
        }

        virtual void RemoveHook(hook::Hook* theHook);

        [[nodiscard]] virtual video::render::Container* GetRenderContainer() const;

        void RemoveChild(Node* node) override;

    protected:
        virtual void OnNodeStateChanged(Node* node);

        virtual void OnNodeAdded(Node* node);

        virtual void OnNodeRemoved(Node* node);

        void addChild(Node* node) override;

        virtual void SetStage(Stage* stage);

    private:
        void addHook(hook::Hook* hook);


        [[nodiscard]] static Node* computeTopDirtyNode(Node* node, const Node* except);

        Stage* stage_;
        std::vector<Node*> activeUpdateNodes_;
        std::vector<Node*> dirtyActiveUpdateNodes_;

        std::vector<Node*> dirtyNodes_;
        std::vector<Node*> dirtyNodes2_;
        std::vector<Node*>* dirtyNodesPtr_;
        std::vector<Node*> nodesToDelete_;

        std::vector<std::vector<Node*>> eventReceiverNodes_;
        std::vector<event::ComponentEventHandler*> componentEventHandler_;
        video::render::Container* renderContainer_;


        std::vector<hook::UpdateHook*> updateHooks_;
        std::vector<hook::RenderHook*> renderHooks_;
        video::render::Pipeline* pipeline_;
    };
} // namespace soil::stage::scene

#endif
