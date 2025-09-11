#ifndef GUI_STAGE_H
#define GUI_STAGE_H
#include "file/font.h"
#include "file/sprite_sheet.h"
#include "menu/item.h"
#include "stage/scene/gui/root.h"
#include "stage/stage.h"

namespace soil_samples::gui {
    class Stage final : public soil::stage::Stage {
    public:
        struct MenuItemDefinition {
            std::string Caption {};
            std::string Value {};
            std::string BackgroundTileName {};
            std::string IconTileName {};
            std::string ToolTip {};
            float LetterSize {1.F};
            Plane::Style BackgroundStyle {HoverStyle};
            const std::function<void(menu::Item& item)> OnClick;
        };

        static inline auto HoverStyle = Plane::Style {
            .BackgroundColor = glm::vec4(0.8F, 0.8F, 0.8F, 1.F),
            .BackgroundColorMouseOver = glm::vec4(1.F),
        };

        explicit Stage();
        ~Stage() override = default;
        void Handle(const soil::WindowEvent& event) override;
        void Render(soil::video::render::State& state) override;
        void Load() override;
        void GenerateMenu(std::vector<MenuItemDefinition> items) const;

    private:
        void initInput(soil::stage::scene::Scene* scene);
        void initGui();
        menu::Item* createMenuItem(const MenuItemDefinition& def) const;
        bool printStatistics_;
        std::vector<soil::video::texture::Texture*> textures_;
        soil::stage::scene::gui::Root* root_;
        soil::file::SpriteSheet spriteSheet_;
        menu::Menu* mainMenu_;
    };
} // namespace soil_samples::gui


#endif
