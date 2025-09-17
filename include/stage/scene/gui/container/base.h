#ifndef SOIL_STAGE_SCENE_GUI_CONTAINER_BASE_H
#define SOIL_STAGE_SCENE_GUI_CONTAINER_BASE_H

#include "stage/scene/gui/rectangle.h"

namespace soil::stage::scene::gui::container {
    class Base : public Rectangle, public soil::event::Handler<event::Node> {
    public:
        ~Base() override;

        [[nodiscard]] const std::vector<Rectangle*>& GetItems() const;
        void Handle(const event::Node& event) override;

        [[nodiscard]] virtual const glm::ivec2& GetOffset() const;
        virtual void SetOffset(const glm::ivec2& offset);
        [[nodiscard]] virtual int GetMargin() const;
        virtual void SetMargin(int margin);
        [[nodiscard]] virtual const glm::ivec2& GetItemsSize() const;
        void RemoveChild(Node* node) override;

    protected:
        explicit Base(int margin = 0, glm::ivec4 padding = glm::ivec4(0));
        void addChild(Node* node) override;
        void addChild(Node* node, bool asItem);
        void BeforeNodeUpdate() override;

        virtual void arrangeItems() = 0;

        int margin_;
        std::vector<Rectangle*> items_;
        glm::ivec2 offset_;
        glm::ivec2 itemsSize_;
    };
} // namespace soil::stage::scene::gui::container

#endif
