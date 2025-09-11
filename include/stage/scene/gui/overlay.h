#ifndef SOIL_STAGE_SCENE_GUI_OVERLAY_H
#define SOIL_STAGE_SCENE_GUI_OVERLAY_H
#include "root.h"

namespace soil::stage::scene::gui {
    class Overlay : public Rectangle {
    public:
        explicit Overlay(Root* root, bool positionRelativeToRoot = false);
        ~Overlay() override = default;
        Rectangle* GetParentRect() const override;
        [[nodiscard]] virtual bool IsPositionRelativeToRoot() const;
        virtual void SetPositionRelativeToRoot(const bool positionRelativeToRoot);

    protected:
        void ComputeWorldTransform(const glm::mat4& parentWorldTransform) override;

    private:
        Root* root_;
        bool positionRelativeToRoot_;
    };
} // namespace soil::stage::scene::gui

#endif
