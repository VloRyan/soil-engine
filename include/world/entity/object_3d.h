#ifndef ENGINE_WORLD_ENTITY_OBJECT_3D_H_
#define ENGINE_WORLD_ENTITY_OBJECT_3D_H_

#include <glm/glm.hpp>

namespace world::entity {
    class Object3d {
    public:
        Object3d(const Object3d &other) = delete;

        Object3d(Object3d &&other) noexcept = delete;

        Object3d();

        virtual ~Object3d();

        [[nodiscard]] virtual glm::vec3 GetPosition() const;

        virtual void SetPosition(glm::vec3 pos);

        [[nodiscard]] virtual glm::vec3 GetWorldPosition() const;

        [[nodiscard]] virtual glm::vec3 GetDirection() const;

        virtual void SetDirection(glm::vec3 direction);

        [[nodiscard]] virtual glm::vec3 GetUp() const;

        virtual void SetUp(glm::vec3 up);

        [[nodiscard]] virtual glm::vec3 GetRight() const;

        virtual void SetRight(glm::vec3 right);

        virtual void SetLocalTransform(const glm::mat4 &Transform);

        [[nodiscard]] virtual const glm::mat4 &GetLocalTransform() const;

        virtual void ComputeWorldTransform(const glm::mat4 &parentWorldTransform);

        [[nodiscard]] const glm::mat4 &GetWorldTransform() const;

    private:
        glm::mat4 localTransform_;
        glm::mat4 worldTransform_;
    };
} // entity

#endif //ENGINE_WORLD_ENTITY_OBJECT_3D_H_
