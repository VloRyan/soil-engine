#ifndef SOIL_STAGE_SCENE_COMPONENT_COMPONENT_H_
#define SOIL_STAGE_SCENE_COMPONENT_COMPONENT_H_
#include <cstdint>
#include <glm/fwd.hpp>
#include <string>

namespace soil::stage::scene {
    class Node;
}

namespace soil::stage::scene::component {
    class Component {
    public:
        enum class Type : std::uint8_t {
            Renderable = 0,
            BoundingVolume,
            Interaction,
            Sound,
            Metadata,
            Lighting,
        };

        enum class State : std::uint8_t {
            Dirty,
            Normal,
            Delete,
        };

        explicit Component(Type type);

        virtual ~Component();

        virtual void SetParent(Node *parent);

        [[nodiscard]] Type GetType() const;

        virtual void Update();

        virtual void UpdateMatrix(const glm::mat4 &matrix);

        [[nodiscard]] State GetState() const;

        [[nodiscard]] std::string GetName() const;

        void SetName(const std::string &name);

        [[nodiscard]] Node *GetParent() const;

        void SetDirty();

        [[nodiscard]] bool IsDirty() const;

    protected:
        void SetState(State state);

    private:
        Node *parent_;
        std::string name_;
        Type type_;
        State state_;
    };
} // namespace soil::stage::scene::component

#endif // SOIL_STAGE_SCENE_COMPONENT_COMPONENT_H_
