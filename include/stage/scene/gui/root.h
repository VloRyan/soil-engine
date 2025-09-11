#ifndef SOIL_STAGE_SCENE_GUI_ROOT_H
#define SOIL_STAGE_SCENE_GUI_ROOT_H

#include "rectangle.h"

namespace soil::stage::scene::gui {
    class Root final : public Rectangle {
    public:
        explicit Root(glm::ivec2 windowSize);
        ~Root() override = default;
        void Handle(const input::Event& event) override;
        void Handle(const WindowEvent& event) override;
    };
} // namespace soil::stage::scene::gui

#endif
