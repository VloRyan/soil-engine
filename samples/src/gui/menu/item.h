#ifndef WILD_SOIL_SCENE_GUI_MENU_ITEM_H
#define WILD_SOIL_SCENE_GUI_MENU_ITEM_H
#include "gui/plane.h"

namespace soil_samples::gui::menu {
    class Menu;

    class Item : public Plane {
    public:
        explicit Item(std::string value = "");
        ~Item() override = default;
        [[nodiscard]] virtual std::string GetValue() const;
        void SetOnClick(const std::function<void(Item& item)>& onClick);

    protected:
        void OnMouseButton(const glm::ivec2& pos, soil::input::MouseButton button,
                           soil::input::Event::State state) override;

    private:
        std::string value_;
        std::function<void(Item& item)> onClick_;
    };


} // namespace soil_samples::gui::menu


#endif
