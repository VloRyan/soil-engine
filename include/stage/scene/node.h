
#ifndef SOIL_STAGE_SCENE_NODE_H
#define SOIL_STAGE_SCENE_NODE_H

#include <bitset>
#include <cstdint>
#include <string>
#include <vector>

#include "component/component.h"
#include "input/event.h"
#include "stage/event/component.h"
#include "stage/event/node.h"
#include "window_event.h"
#include "world/entity/object_3d.h"

namespace soil::stage::scene {
    class Scene;


    class Node : public world::entity::Object3d, public input::EventHandler, public WindowEventHandler {
    public:
        enum class State : std::uint8_t {
            /** Needs to be (re)initialized */
            Dirty,
            /** Normal */
            Normal,
            /** Will be deleted */
            Delete,
        };

        enum class UpdateType : std::uint8_t {
            Active,
            Passive,
        };

        enum class DirtyCauses : std::uint8_t {
            Matrix = 0,
            Component,
            Data,
            UpdateType,
        };

        enum class Type : std::uint8_t {
            Scene = 0,
            Viewer,
            Visual,
            Game,
            Sound,
            Transform,
            Input,
        };


        enum class ReceiverType : std::uint8_t { None = 0, Window, Input };

        explicit Node(Node *parent, Type type, const std::vector<ReceiverType> &receiveTypeFlags = {},
                      std::string name = "");

        ~Node() override;

        [[nodiscard]] bool IsDirty() const;

        [[nodiscard]] bool IsDirtyCause(DirtyCauses cause) const;

        [[nodiscard]] State GetState() const;

        virtual void SetState(State state);

        virtual void SetDirty(DirtyCauses cause);

        [[nodiscard]] bool IsState(State state) const;

        [[nodiscard]] Type GetType() const;

        void SetParent(Node *parent);

        [[nodiscard]] Node *GetParent() const;

        [[nodiscard]] const std::vector<Node *> &GetChildren() const;

        [[nodiscard]] component::Component *GetComponent(component::Component::Type type) const;

        [[nodiscard]] std::vector<component::Component *> GetComponents(component::Component::Type type) const;

        template <class T>
        T AddComponent(T comp) {
            using type = std::remove_pointer_t<T>;
            static_assert(std::is_base_of_v<component::Component, type>,
                          "comp must be derived from component::Component");
            addComponent(comp);
            return comp;
        }

        void RemoveComponent(component::Component *comp);

        [[nodiscard]] std::bitset<4> GetReceiveTypes() const;

        virtual void Update();

        [[nodiscard]] virtual std::string GetName() const;

        virtual void SetName(const std::string &name);

        [[nodiscard]] Scene *GetScene() const;

        void SetPosition(glm::vec3 pos) override;

        void SetDirection(glm::vec3 direction) override;

        void SetRight(glm::vec3 right) override;

        void SetUp(glm::vec3 up) override;

        void SetLocalTransform(const glm::mat4 &Transform) override;

        void Handle(const input::Event &event) override {}

        void Handle(const WindowEvent &event) override {}

        virtual void Handle(const event::Component &event);

        [[nodiscard]] UpdateType GetUpdateType() const;

        [[nodiscard]] int GetId() const;

    protected:
        explicit Node(int id, Type type, const std::vector<ReceiverType> &receiveTypeFlags = {}, std::string name = "");

        void SetScene(Scene *root);

        void fire(const event::Node &evnt) const;

        virtual void UpdateDirty();

        virtual void AddChild(Node *node);

        virtual void RemoveChild(Node *node);

        [[nodiscard]] bool isScene() const;

        void SetUpdateType(UpdateType type);

        void addComponent(component::Component *comp);

    private:
        int id_;
        Node *parent_;
        Scene *scene_;
        std::string name_;
        Type type_;
        State state_;
        UpdateType updateType_;
        std::bitset<4> dirtyCause_;
        std::vector<Node *> children_;
        std::vector<std::vector<component::Component *> > components_;
        std::bitset<4> receiveTypeFlags_;
    };
} // namespace soil::stage::scene
#endif // SOIL_STAGE_SCENE_NODE_H
