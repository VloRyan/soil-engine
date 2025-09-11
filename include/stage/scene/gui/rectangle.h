#ifndef SOIL_STAGE_SCENE_GUI_RECTANGLE_H
#define SOIL_STAGE_SCENE_GUI_RECTANGLE_H

#include "stage/scene/node.h"
#include "video/render/state.h"

namespace soil::stage::scene::gui {
    class Rectangle : public Node {
        friend class Root;

    public:
        enum class HorizontalAnchors { None = 0, Left, Center, Right };
        enum class VerticalAnchors { None = 0, Top, Middle, Bottom };

        static inline auto LAYER_Z_INCREMENT = 0.1F;
        static inline auto LAYER_Z_COMPONENT_INCREMENT = 0.01F;
        static inline auto TOP_Z_LAYER = 99.9F;

        explicit Rectangle();
        ~Rectangle() override = default;
        [[nodiscard]] virtual Rectangle* GetParentRect() const;

        virtual void SetSize(const glm::ivec2& size);
        [[nodiscard]] virtual const glm::ivec2& GetSize() const;
        [[nodiscard]] virtual const glm::vec2& GetRelativeSize() const;
        virtual void SetRelativeSize(const glm::vec2& relative_size);

        [[nodiscard]] virtual glm::ivec2 GetChildSize() const;

        [[nodiscard]] virtual glm::vec2 GetCenter() const;

        void SetAnchor(HorizontalAnchors horizontal, VerticalAnchors vertical);
        [[nodiscard]] virtual const glm::vec2& GetAspectRatio() const;
        virtual void SetAspectRatio(const glm::vec2& aspectRatio);
        [[nodiscard]] virtual bool IsMouseOver() const;

        [[nodiscard]] bool Contains(glm::ivec2 pos) const;
        [[nodiscard]] bool Contains(const Rectangle* other) const;


        virtual void SetVisible(bool visible);
        virtual bool IsVisible() const;

        virtual void SetOnMouseOverFunc(const std::function<void(glm::ivec2 pos)>& onMouseOverFunc);
        virtual void SetOnMouseOutFunc(const std::function<void()>& onMouseOutFunc);
        [[nodiscard]] virtual const video::render::Rect& GetScissorRect() const;
        virtual void UpdateSize(const glm::ivec2& parentSize);

        [[nodiscard]] virtual const glm::ivec4& GetPadding() const;
        virtual void SetPadding(const glm::ivec4& padding);
        void RemoveChild(Node* node) override;

    protected:
        void addChild(Node* node) override;
        virtual void addChildRect(Rectangle* rect);
        virtual void removeChildRect(const Rectangle* rect);
        void UpdateDirty() override;
        virtual void BeforeNodeUpdate();
        virtual void UpdateScissor(const video::render::Rect& parentRect);

        void ApplyAnchors(const glm::ivec2& parentSize, const glm::vec2& parentCenter);

        virtual void OnMouseOver(const glm::ivec2& pos);
        virtual void OnMouseOut();
        virtual void OnMouseButton(const glm::ivec2& pos, input::MouseButton button, input::Event::State state);
        virtual void OnMouseWheel(const glm::ivec2& pos, glm::vec2 offset);

        std::vector<Rectangle*> children_;
        bool isMouseOver_;


        glm::ivec2 size_;
        glm::ivec2 minSize_;
        glm::vec2 relativeSize_;
        glm::vec2 aspectRatio_;
        video::render::Rect scissorRect_;
        video::render::Rect childScissorRect_;

        glm::ivec4 padding_;

        HorizontalAnchors horizontalAnchors_;
        VerticalAnchors verticalAnchors_;
        bool visible_;
        bool visibleEffective_;

        std::function<void(glm::ivec2 pos)> onMouseOverFunc_;
        std::function<void()> onMouseOutFunc_;
    };
} // namespace soil::stage::scene::gui

#endif
