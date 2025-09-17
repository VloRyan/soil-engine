#include "stage.h"

#include <asset.h>
#include <plog/Log.h>
#include <stage/scene/input.h>
#include <string>

#include "basic/shader.h"
#include "basic/shape.h"
#include "character_shader.h"
#include "component/shape_tile_instance.h"
#include "component/text.h"
#include "file/font.h"
#include "label.h"
#include "menu/item.h"
#include "menu/menu.h"
#include "shape_tile_shader.h"
#include "stage/scene/gui/container/h_box.h"
#include "stage/scene/gui/overlay.h"
#include "stage/scene/gui/root.h"
#include "stage/scene/render/plain.h"
#include "stage/scene/render/update_matrices_ubo.h"
#include "stage/scene/scene.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"

namespace soil_samples::gui {
    constexpr auto UBO_TARGET_MATRICES = 0;

    Stage::Stage() : printStatistics_(false), root_(nullptr), mainMenu_(nullptr) {}

    void Stage::OnLoad() {
        auto* scene = AddScene(new soil::stage::scene::Scene());
        scene->SetPipeline(soil::video::render::Pipeline::NewForwardRenderingPipeline(scene->GetRenderContainer()));


        auto* fontFile = soil::file::Font::Load(asset::GetPath("Fonts/Calibri.fnt"));
        auto* fontTexture = GetResources().Textures().GetTexture2D(fontFile->TextureFileName);

        spriteSheet_ = soil::file::SpriteSheet::Load(asset::GetPath("Textures/Tiles/Gui.json"));
        auto* guiTexture =
            GetResources().Textures().GetTextureArray2D(spriteSheet_.GetTextureFile(), spriteSheet_.TilesPerDim);
        textures_ = {fontTexture, guiTexture};


        auto& state = GetResources().GetRenderState();
        for (auto* texture : textures_) {
            state.SetTexture(*texture);
        }
        const auto viewer =
            scene->AddChild(new soil::stage::scene::viewer::Ortho(GetResources().GetWindow()->GetSize()));
        viewer->Look(glm::vec3(0.F), glm::vec3(0.F, 0.F, -1.F));


        scene->AddHook(new soil::stage::scene::render::UpdateMatricesUbo(viewer, UBO_TARGET_MATRICES));

        auto* quadMesh = GetResources().GetMesh({.Identifier = "Quad"});

        auto* shapeTileShader = dynamic_cast<ShapeTileShader*>(GetResources().GetShader(ShapeTileShader::NAME));
        shapeTileShader->SetViewer(viewer); // will update PV matrix in Shader::Prepare())

        scene->AddHook(new soil::stage::scene::render::Plain(scene->GetRenderContainer()));


        root_ = scene->AddChild(new soil::stage::scene::gui::Root(GetResources().GetWindow()->GetSize()));

        auto* charShader = dynamic_cast<CharacterShader*>(GetResources().GetShader(CharacterShader::NAME));
        charShader->SetViewer(viewer);

        component::ShapeTile::InitPrefab("gui",
                                         {
                                             .MeshData = quadMesh,
                                             .Shader = shapeTileShader,
                                             .Texture = guiTexture,
                                         });
        soil::stage::scene::component::text::AbstractText::InitPrefab("Calibri",
                                                                      {
                                                                          .MeshData = quadMesh,
                                                                          .Shader = charShader,
                                                                          .Font = fontFile,
                                                                          .FontTexture = fontTexture,
                                                                      });

        initGui();
    }

    void Stage::RegisterInputEvents(soil::input::EventMap& eventMap) {
        eventMap
            .AddKeyMapping(soil::input::Keys::Escape, soil::input::Event::State::Release,
                           [this](const soil::input::Event&) { GetResources().GetWindow()->Close(); })
            .AddKeyMapping(soil::input::Keys::S, soil::input::Event::State::Release,
                           [this](const soil::input::Event&) { printStatistics_ = !printStatistics_; });
    }

    void Stage::GenerateMenu(const std::vector<MenuItemDefinition>& items) const {
        for (const auto& menuItem : items) {
            auto* item = createMenuItem(menuItem);
            mainMenu_->AddChild(item);
        }
        const auto exitButton =
            MenuItemDefinition{.Caption = "Exit",
                               .Value = "exit",
                               .BackgroundTileName = "button",
                               .IconTileName = "exit",
                               .ToolTip = "Exit the program",
                               .LetterSize = 0.5f,
                               .OnClick = [this](menu::Item&) { GetResources().GetWindow()->Close(); }};
        auto* item = createMenuItem(exitButton);
        mainMenu_->AddChild(item);
    }

    void Stage::Render(soil::video::render::State& state) {
        soil::stage::Stage::Render(state);
    }

    void Stage::initGui() {
        auto* backgroundPlane = root_->AddChild(new Plane());
        backgroundPlane->SetRelativeSize(glm::vec2(1.F));
        auto* bgIcons = backgroundPlane->AddChild(new Plane());
        bgIcons->SetRelativeSize(glm::vec2(0.8F));
        bgIcons->Background().SetTileIndex(spriteSheet_.TileIndex("bg"));
        bgIcons->Background().SetTileScale(glm::vec2(10.F));
        bgIcons->SetRelativeSize(glm::vec2(1.0F));

        mainMenu_ = bgIcons->AddChild(new menu::Menu({
            .Padding = glm::vec4(10.F),
            .Scrollbar = {},
            .SpriteSheet = &spriteSheet_,
            .BackgroundTileName = "darkBg",
            .Style =
                {
                    .BackgroundColor = glm::vec4(0.8F, 0.8F, 0.8F, 0.95F),
                    .BackgroundColorMouseOver = glm::vec4(1.F),
                },
        }));
        mainMenu_->SetAnchor(soil::stage::scene::gui::Rectangle::HorizontalAnchors::Center,
                             soil::stage::scene::gui::Rectangle::VerticalAnchors::Middle);
        mainMenu_->SetRelativeSize(glm::vec2(0.0F, 0.6F));
        mainMenu_->SetAspectRatio(glm::vec2(0.5F, 0.F));
    }

    menu::Item* Stage::createMenuItem(const MenuItemDefinition& def) const {
        auto* item = new menu::Item(def.Value);
        item->Background().SetTileIndex(spriteSheet_.TileIndex(def.BackgroundTileName));
        item->SetStyle(def.BackgroundStyle);
        item->SetRelativeSize(glm::vec2(0.95F, 0.0F));
        item->SetAspectRatio(glm::vec2(0.F, 4.F / 1.F));
        item->SetOnClick(def.OnClick);
        auto* container =
            item->AddChild(new soil::stage::scene::gui::container::HBox(10.F, glm::vec4(10.F, 0.F, 0.F, 0.F)));
        container->SetRelativeSize(glm::vec2(1.F));
        container->SetAnchor(soil::stage::scene::gui::Rectangle::HorizontalAnchors::Left,
                             soil::stage::scene::gui::Rectangle::VerticalAnchors::Middle);
        auto* itemIcon = container->AddChild(new Plane());
        itemIcon->SetRelativeSize(glm::vec2(0.F, 0.85F));
        itemIcon->SetAspectRatio(glm::vec2(1.F, 0.F));
        itemIcon->Background().SetTileIndex(spriteSheet_.TileIndex(def.IconTileName));

        const auto* label = container->AddChild(new Label(def.Caption));
        label->Text().SetCharacterSize(def.LetterSize);

        if (!def.ToolTip.empty()) {
            auto* toolTip = item->AddChild(new soil::stage::scene::gui::Overlay(root_, true));
            toolTip->SetVisible(false);
            auto* toolTipLabel = toolTip->AddChild(new Label(def.ToolTip));
            auto style = toolTipLabel->GetStyle();
            style.BackgroundColor = glm::vec4(0.5F, 0.5F, 0.5F, 0.8F);
            style.BackgroundColorMouseOver = glm::vec4(0.5F, 0.5F, 0.5F, 0.8F);
            toolTipLabel->SetStyle(style);
            toolTipLabel->Text().SetCharacterSize(0.2F);
            item->SetOnMouseOverFunc([toolTip, toolTipLabel](const glm::ivec2 pos) {
                if (!toolTip->IsVisible()) {
                    toolTip->SetSize(toolTipLabel->GetSize());
                    toolTip->SetVisible(true);
                }
                const auto halfSize = toolTipLabel->GetSize() / glm::ivec2(2.F, 2.F);
                toolTip->SetPosition(glm::vec3(pos + halfSize, 0.F));
            });
            item->SetOnMouseOutFunc([toolTip] { toolTip->SetVisible(false); });
        }

        return item;
    }

    void Stage::Handle(const soil::WindowEvent& event) {
        soil::stage::Stage::Handle(event);
        if (printStatistics_ && event.GetCause() == soil::WindowEvent::StatisticsChanged) {
            const auto stats = event.GetWindow()->GetStatistics();
            PLOG_DEBUG << "FPS: " << std::to_string(stats.FPS)
                       << " Draws: " << std::to_string(stats.DrawCount / stats.FPS)
                       << " Vertices: " << std::to_string(stats.VertexCount / stats.FPS)
                       << " State changes: " << std::to_string(stats.StateChanges / stats.FPS)
                       << " Update times: " << std::to_string(stats.updateInputTime / stats.FPS) << ", "
                       << std::to_string(stats.updateStageTime / stats.FPS) << ", "
                       << std::to_string(stats.updateVideoTime / stats.FPS)
                       << " Render times: " << std::to_string(stats.startRenderTime / stats.FPS) << ", "
                       << std::to_string(stats.renderTime / stats.FPS) << ", "
                       << std::to_string(stats.endRenderTime / stats.FPS);
        }
    }


} // namespace soil_samples::gui
