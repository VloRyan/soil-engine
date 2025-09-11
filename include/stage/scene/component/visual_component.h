#ifndef SOIL_STAGE_SCENE_COMPONENT_VISUAL_COMPONENT_HPP
#define SOIL_STAGE_SCENE_COMPONENT_VISUAL_COMPONENT_HPP
#include "component.h"
#include "stage/scene/render/type.hpp"
#include "video/render/renderable_object.h"

namespace soil::stage::scene::component {
    class VisualComponent : public Component {
    public:
        ~VisualComponent() override = default;

        [[nodiscard]] virtual render::Type GetRenderType() const;

        [[nodiscard]] virtual bool IsVisible() const;
        virtual void SetVisible(bool visible);

        [[nodiscard]] bool IsOpaque() const;
        virtual void SetOpaque(bool opaque);

        [[nodiscard]] virtual bool IsCulled() const;
        virtual void SetCulled(bool culled);

        [[nodiscard]] virtual video::render::RenderableObject* GetRenderable() = 0;

        static VisualComponent* Cast(Component* component);
        static bool IsRenderType(Component* component, render::Type renderType);

    protected:
        explicit VisualComponent(render::Type renderType, bool opaque = true, bool visible = true);

    private:
        render::Type renderType_;
        bool visible_;
        bool opaque_;
        bool culled_;
    };
} // namespace soil::stage::scene::component
#endif
