#ifndef SOIL_STAGE_SCENE_GUI_CONTAINER_V_BOX_H
#define SOIL_STAGE_SCENE_GUI_CONTAINER_V_BOX_H
#include "base.h"

namespace soil::stage::scene::gui::container {
    class VBox : public Base {
    public:
        enum class AlignItems : std::int8_t {
            Left = 0,
            Center,
            Right,
        };
        explicit VBox(int margin = 0, glm::ivec4 padding = glm::ivec4(0));
        ~VBox() override = default;
        [[nodiscard]] virtual AlignItems GetAlignItems() const;
        virtual void SetAlignItems(AlignItems alignItems);

    protected:
        void arrangeItems() override;

    private:
        AlignItems alignItems_;
    };
} // namespace soil::stage::scene::gui::container

#endif
