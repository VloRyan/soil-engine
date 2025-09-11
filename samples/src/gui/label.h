#ifndef WILD_SOIL_SCENE_GUI_LABEL_H
#define WILD_SOIL_SCENE_GUI_LABEL_H
#include "component/text.h"
#include "plane.h"
#include "text/node.h"

namespace soil_samples::gui {
    class Label final : public Plane {
    public:
        explicit Label(const std::string& text = "", const std::string& font = "Calibri");
        ~Label() override = default;
        void SetVisible(bool visible) override;
        void SetText(const std::string& text) const;
        component::Text& Text() const;

    protected:
        void BeforeNodeUpdate() override;

    private:
        component::Text* text_;
    };
} // namespace soil_samples::gui

#endif
