#ifndef SOIL_STAGE_SCENE_COMPONENT_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_COMPONENT_H
#include <glm/glm.hpp>

#include "event/observable.hpp"
#include "stage/event/component.h"

namespace soil::stage::scene {
    class Node;
}

namespace soil::stage::scene::component {
    class Component : public soil::event::Observable<event::Component> {
        friend class scene::Node;

    public:
        enum class Type : std::uint8_t {
            Visual = 0,
            BoundingVolume,
            Interaction,
            Sound,
            Metadata,
            Lighting,
            World,
            Any = 255
        };

        enum class State : std::uint8_t {
            Dirty = 0,
            Normal,
            Delete,
        };

        explicit Component(Type type);

        ~Component() override;

        [[nodiscard]] Node* GetParent() const;

        [[nodiscard]] Type GetType() const;

        virtual void Update();

        [[nodiscard]] State GetState() const;

        [[nodiscard]] bool IsDirty() const;

    protected:
        virtual void SetDirty();

        void SetState(State state);

        virtual void UpdateTransform(const glm::mat4& transform);

        virtual void SetParent(Node* parent);

    private:
        Node* parent_;
        Type type_;
        State state_;
    };
} // namespace soil::stage::scene::component

#endif
