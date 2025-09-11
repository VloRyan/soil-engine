#ifndef WILD_SOIL_SCENE_GUI_MENU_MENU_H
#define WILD_SOIL_SCENE_GUI_MENU_MENU_H

#include "file/sprite_sheet.h"
#include "gui/plane.h"
#include "scrollbar.h"
#include "stage/scene/gui/container/base.h"
#include "stage/scene/gui/rectangle.h"

namespace soil_samples::gui::menu {
    class Item;

    class Menu : public soil::stage::scene::gui::Rectangle {
    public:
        enum class Orientation : std::uint8_t { Vertical = 0, Horizontal };

        struct ScrollBarDefinition {
            bool Enabled{true};
            int Width{30};
        };

        struct Definition {
            Menu::Orientation Orientation{Orientation::Vertical};
            int Margin{10};
            glm::ivec4 Padding{0};
            ScrollBarDefinition Scrollbar{.Enabled = false};
            soil::file::SpriteSheet* SpriteSheet{nullptr};
            std::string BackgroundTileName;
            Plane::Style Style{};
        };

        explicit Menu(const Definition& definition);
        ~Menu() override = default;

        [[nodiscard]] virtual glm::vec2 GetScrollOffset() const;
        virtual void SetScrollOffset(const glm::vec2& scrollOffset);
        [[nodiscard]] virtual int GetSelectedItemIndex() const;
        virtual void SetSelectedItemIndex(int selectedItemIndex);
        void OnSelectionChanged(const std::function<void(int current, int prev)>& onSelectionChanged);
        [[nodiscard]] virtual Item* GetItem(int index) const;
        void SetSize(const glm::ivec2& size) override;

        glm::ivec2 GetChildSize() const override;
        glm::vec2 GetCenter() const override;

    protected:
        void addChild(Node* node) override;
        void UpdateScissor(const soil::video::render::Rect& parentRect) override;

        void OnMouseOver(const glm::ivec2& pos) override;
        void OnMouseOut() override;
        void OnMouseWheel(const glm::ivec2& pos, glm::vec2 offset) override;
        void OnMouseButton(const glm::ivec2& pos, soil::input::MouseButton button,
                           soil::input::Event::State state) override;

        void BeforeNodeUpdate() override;
        Definition definition_;
        Plane* bgPlane_;
        glm::ivec2 scrollOffset_;
        Scrollbar* verticalScrollbar_;
        int selectedItemIndex_;
        int maxItems_;
        std::function<void(int current, int prev)> onSelectionChanged_;
        soil::stage::scene::gui::container::Base* container_;
    };
} // namespace soil_samples::gui::menu


#endif
