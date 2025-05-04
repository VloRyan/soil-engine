#ifndef ENGINE_STAGE_SCENE_VIEWER_FPS_CAMERA_H
#define ENGINE_STAGE_SCENE_VIEWER_FPS_CAMERA_H
#include "perspective.h"

namespace stage::scene::viewer {
    class FPSCamera final : public Perspective {
    public:
        explicit FPSCamera(scene::Node *parent, glm::ivec2 resolution);

        ~FPSCamera() override;

    protected:
        void Update() override;

        void updateRotation();

        void Handle(const input::Event &event) override;

    private:
        float mouseSpeed_;
        bool mouseControlled_;
        glm::vec2 cursorPosition_;
        glm::vec2 cursorPositionDiff_;
    };
} // stage

#endif //ENGINE_STAGE_SCENE_VIEWER_FPS_CAMERA_H
