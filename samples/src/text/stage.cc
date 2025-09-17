#include "stage.h"

#include <asset.h>
#include <stage/scene/input.h>
#include <string>

#include "basic/shader.h"
#include "basic/shape.h"
#include "glm/glm.hpp"
#include "instancing/shader.h"
#include "instancing/shape_instance.h"
#include "node.h"
#include "stage/scene/scene.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"

#include "gui/character_shader.h"
#include "gui/component/text.h"
#include "stage/scene/render/plain.h"

namespace soil_samples::text {
    Stage::Stage() :
        printStatistics_(false), text_(nullptr), bgNode_(nullptr), bgShape_(nullptr), description_(),
        bounceText_(nullptr), bounceTextVelocity(0.F), bounceTextGlowVelocity(0.01F) {}

    void Stage::OnLoad() {
        auto* scene = AddScene(new soil::stage::scene::Scene());
        scene->SetPipeline(soil::video::render::Pipeline::NewForwardRenderingPipeline(scene->GetRenderContainer()));

        const auto viewer =
            scene->AddChild(new soil::stage::scene::viewer::Ortho(GetResources().GetWindow()->GetSize()));
        auto* quadMesh = GetResources().GetMesh({.Identifier = "Quad"});
        auto* instancing = scene->AddHook(new soil::stage::scene::render::Instancing(scene->GetRenderContainer()));
        auto* bgTexture = GetResources().Textures().GetTexture2D(asset::GetPath("Textures/crt.jpg"));
        auto& renderState = GetResources().GetRenderState();
        auto* bgShader = dynamic_cast<instancing::Shader*>(GetResources().GetShader(instancing::Shader::NAME));
        bgShader->SetTextures({bgTexture});
        renderState.SetTexture(0, *bgTexture);
        bgShader->SetViewer(viewer); // will update PV matrix in Shader::Prepare())

        instancing->AddRenderBatch(instancing::ShapeInstance::BATCH_NAME,
                                   {
                                       .Mesh = quadMesh,
                                       .Shader = bgShader,
                                       .VertexAttribDescriptors = instancing::ShapeInstance::ATTRIBS,
                                       .State = {.DepthTest = true},
                                   });
        initBackground(scene, 0);

        scene->AddHook(new soil::stage::scene::render::Plain(scene->GetRenderContainer()));

        auto* fontFile = soil::file::Font::Load(asset::GetPath("Fonts/Calibri.fnt"));
        auto* fontTexture = GetResources().Textures().GetTexture2D(fontFile->TextureFileName);
        renderState.SetTexture(1, *fontTexture);

        auto* charShader = dynamic_cast<gui::CharacterShader*>(GetResources().GetShader(gui::CharacterShader::NAME));
        charShader->Use();
        charShader->SetViewer(viewer); // will update PV matrix in Shader::Prepare())

        soil::stage::scene::component::text::AbstractText::InitPrefab("TextCalibri",
                                                                      {
                                                                          .MeshData = quadMesh,
                                                                          .Shader = charShader,
                                                                          .Font = fontFile,
                                                                          .FontTexture = fontTexture,
                                                                      });

        text_ = scene->AddChild(new Node("TextCalibri", "Hello world!"));
        text_->Text().SetCharacterSize(1);
        text_->Text().SetMaxSize(glm::vec2(300.0f));
        text_->Text().SetColor(glm::vec4(0.0, .4F, .4F, .5F));
        text_->Text().SetBorderColor(glm::vec3(0.0, .6F, .6F));
        text_->Text().SetCharacterOutline(glm::vec2(0.5F, 0.2F));
        text_->Text().SetBorderOutline(glm::vec2(0.5F, 0.2F));

        description_ = scene->AddChild(new Node("TextCalibri",
                                                "Text sample\n"
                                                "Press\n"
                                                "    s - Show statistics and FPS\n"
                                                "    t - Toggle long text (lorem ipsum)\n"
                                                "    + - Increase text size\n"
                                                "    - - Decrease text size\n"));
        description_->Text().SetCharacterSize(0.2);
        description_->Text().SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        const auto winSize = glm::vec2(GetResources().GetWindow()->GetSize());
        const auto winCenter = glm::vec2(winSize) * glm::vec2(.5F);

        bounceText_ = scene->AddChild(new Node("TextCalibri", "Bouncing..."));
        bounceText_->Text().SetCharacterSize(1);
        bounceText_->Text().SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        bounceText_->Text().SetCharacterOutline(glm::vec2(0.5F, 0.2F));
        bounceText_->Text().SetBorderOutline(glm::vec2(0.5F, 0.2F));
        bounceText_->SetPosition(glm::vec3(winCenter, -0.5F));
        bounceText_->Text().SetBorderColor(glm::vec3(1, 0, 0));

        bounceTextVelocity = glm::vec2(4.F, -4.F);

        description_->Update();
        bgShape_->SetSize(winSize);
        bgNode_->SetPosition(glm::vec3(winCenter + glm::vec2(15), -1.F));
        text_->SetPosition(glm::vec3(winCenter, -0.5F));
        description_->SetPosition(glm::vec3(description_->Text().GetSize().x * 0.5F + 20,
                                            winSize.y - description_->Text().GetSize().y * 0.5F, -0.5));
    }

    void Stage::RegisterInputEvents(soil::input::EventMap& eventMap) {
        eventMap
            .AddKeyMapping(soil::input::Keys::Key_Plus, soil::input::Event::State::Press,
                           [this](const soil::input::Event&) {
                               text_->Text().SetCharacterSize(text_->Text().GetCharacterSize() + 1.F);
                           })
            .AddKeyMapping(soil::input::Keys::Key_Minus, soil::input::Event::State::Press,
                           [this](const soil::input::Event&) {
                               if (text_->Text().GetCharacterSize() > 1.F) {
                                   text_->Text().SetCharacterSize(text_->Text().GetCharacterSize() - 1.F);
                               };
                           })
            .AddKeyMapping(soil::input::Keys::S, soil::input::Event::State::Release,
                           [this](const soil::input::Event&) {
                               printStatistics_ = !printStatistics_;
                               if (!printStatistics_) {
                                   text_->Text().SetText("Hallo world!");
                               }
                           })
            .AddKeyMapping(
                soil::input::Keys::L, soil::input::Event::State::Release,
                [this](const soil::input::Event&) {
                    if (text_->Text().GetText().starts_with("Lorem")) {
                        text_->Text().SetText("Hallo world!");
                    } else {
                        text_->Text().SetText(
                            "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor "
                            "invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et "
                            "accusam "
                            "et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est "
                            "Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed "
                            "diam "
                            "nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. "
                            "At "
                            "vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea "
                            "takimata sanctus est Lorem ipsum dolor sit amet.");
                    }
                })
            .AddKeyMapping(soil::input::Keys::F, soil::input::Event::State::Release, [this](const soil::input::Event&) {
                if (fastChangeIndex == -1) {
                    fastChangeIndex = 0;
                } else {
                    fastChangeIndex = -1;
                }
            });
    }

    void Stage::initBackground(soil::stage::scene::Scene* scene, const int textureSlot) {
        const auto winSize = glm::vec2(GetResources().GetWindow()->GetSize() - glm::ivec2(30, 30));
        bgNode_ = scene->AddChild(new soil::stage::scene::Node(soil::stage::scene::Node::Type::Visual));
        bgShape_ = bgNode_->AddComponent(new instancing::ShapeInstance(true));
        bgShape_->SetSize(winSize);
        bgShape_->SetTextureIndex(textureSlot);
        bgShape_->SetColor({.2F, .2F, .2F, 1.F});
    }

    void Stage::Handle(const soil::WindowEvent& event) {
        soil::stage::Stage::Handle(event);
        if (printStatistics_ && event.GetCause() == soil::WindowEvent::StatisticsChanged) {
            const auto stats = event.GetWindow()->GetStatistics();
            text_->Text().SetText(
                "Hello world!\n"
                "FPS:" +
                std::to_string(stats.FPS) +
                "\n"
                " Draws: " +
                std::to_string(stats.DrawCount / stats.FPS) +
                "\n"
                " Vertices: " +
                std::to_string(stats.VertexCount / stats.FPS) +
                "\n"
                " State changes: " +
                std::to_string(stats.StateChanges / stats.FPS) +
                "\n"
                " Update times: " +
                std::to_string(stats.updateInputTime / stats.FPS) + ", " +
                std::to_string(stats.updateStageTime / stats.FPS) + ", " +
                std::to_string(stats.updateVideoTime / stats.FPS) +
                "\n"
                " Render times: " +
                std::to_string(stats.startRenderTime / stats.FPS) + ", " +
                std::to_string(stats.renderTime / stats.FPS) + ", " + std::to_string(stats.endRenderTime / stats.FPS));
        }
        if (event.GetCause() == soil::WindowEvent::SizeChanged) {
            const auto winSize = glm::vec2(event.GetWindow()->GetSize() - glm::ivec2(30, 30));
            const auto winCenter = glm::vec2(winSize) * glm::vec2(.5F);
            bgShape_->SetSize(winSize);
            bgNode_->SetPosition(glm::vec3(winCenter + glm::vec2(15), -1.F));
            text_->SetPosition(glm::vec3(winCenter, -0.5F));
            description_->SetPosition(glm::vec3(description_->Text().GetSize().x * 0.5F + 20,
                                                winSize.y - description_->Text().GetSize().y * 0.5F, -0.5));
        }
    }

    void Stage::Update() {
        const auto winSize = glm::vec2(GetResources().GetWindow()->GetSize() - glm::ivec2(30, 30));
        const auto pos = bounceText_->GetPosition();
        auto newPos = glm::vec2(pos) + bounceTextVelocity;
        auto halfSize = bounceText_->Text().GetSize() * glm::vec2(0.5F);
        for (auto i = 0; i < 2; i++) {
            if (newPos[i] + halfSize[i] > winSize[i]) {
                newPos[i] = winSize[i] - halfSize[i];
                bounceTextVelocity[i] *= -1;
            }
            if (newPos[i] - halfSize[i] < 0) {
                newPos[i] = 0 + halfSize[i];
                bounceTextVelocity[i] *= -1;
            }
        }
        bounceText_->SetPosition(glm::vec3(newPos, pos.z));
        auto outline = bounceText_->Text().GetBorderOutline();
        outline.x += bounceTextGlowVelocity;
        if (outline.x > 0.8F) {
            bounceTextGlowVelocity = -0.01F;
            outline.x = 0.8F;
        }
        if (outline.x < 0.6F) {
            bounceTextGlowVelocity = 0.01F;
            outline.x = 0.6F;
        }
        bounceText_->Text().SetBorderOutline(outline);
        const std::vector<std::string> parts = {"lorem", "ipsum", "dolor", "sit", "amet"};
        if (fastChangeIndex != -1) {
            text_->Text().SetText(parts[fastChangeIndex++]);
            if (fastChangeIndex == parts.size()) {
                fastChangeIndex = 0;
            }
        }
        soil::stage::Stage::Update();
    }


} // namespace soil_samples::text
