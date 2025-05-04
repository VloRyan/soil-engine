#ifndef ENGINE_STAGE_SCENE_VIEWER_PERSPECTIVE_H_
#define ENGINE_STAGE_SCENE_VIEWER_PERSPECTIVE_H_

#include "node.h"
#include "window.h"
#include "window_event.h"
#include "glm/glm.hpp"

namespace stage::scene::viewer {
    class Perspective : public Node {
    public:
        explicit Perspective(scene::Node *parent, glm::ivec2 resolution);

        ~Perspective() override;

        [[nodiscard]] glm::mat4 GetProjectionMatrix() const override;

        [[nodiscard]] glm::mat4 GetViewMatrix() const override;

        world::collision::Frustum* GetFrustum() override;

        void Look(glm::vec3 center, glm::vec3 up = glm::vec3(0.0F, 1.0F, 0.0F));

        void Move(glm::vec3 move);

        void AddVelocity(glm::vec3 velocity, bool relative = true);

        //virtual void UpdateRotation(float timeElapsed) = 0;

        void SetPerspective(float fovY, float aspect, float nearZ, float farZ);

        void Handle(const engine::WindowEvent &event) override;

        [[nodiscard]] glm::vec3 GetRotate() const;

        void SetRotate(glm::vec3 rot);


        [[nodiscard]] glm::vec3 GetRight() const override;

        [[nodiscard]] glm::vec3 GetUp() const override;

        [[nodiscard]] glm::vec3 GetDirection() const override;

        void SetRight(glm::vec3 right) override;

        void SetUp(glm::vec3 up) override;

        void SetPosition(glm::vec3 pos) override;

        void SetDirection(glm::vec3 direction) override;

        void Update() override;

    protected:
        void UpdateDirty();


        bool updateVelocity_;
        glm::vec3 moveSpeed_;
        glm::vec3 direction_;
        glm::vec3 right_;
        glm::vec3 up_;
        glm::vec3 rotate_;
        //glm::vec3 position_;
        glm::vec3 velocity_;

        float fovY_;
        float nearZ_;
        float farZ_;

        glm::mat4 perspective_;
        glm::mat4 view_;
        world::collision::Frustum *frustum_;
    };
} // stage::scene::viewer

#endif //ENGINE_STAGE_SCENE_VIEWER_PERSPECTIVE_H_
