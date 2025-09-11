#include "stage.h"

#include <asset.h>
#include <plog/Log.h>
#include <stage/scene/input.h>
#include <string>

#include "basic/shader.h"
#include "basic/shape.h"
#include "common/rotation_node.h"
#include "glm/glm.hpp"
#include "stage/scene/render/plain.h"
#include "stage/scene/scene.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/scene/volume/frustum_culling.h"
#include "stage/stage.h"
#include "world/volume/aabb.h"
#include "world/volume/quad_tree.h"

namespace soil_samples::volume {
    Stage::Stage() : printStatistics_(false), container_(nullptr), shapes_() {}

    void Stage::Load() {
        auto* scene = AddScene(new soil::stage::scene::Scene());
        scene->AddHook(new soil::stage::scene::render::Plain(scene->GetRenderContainer()));

        initInput(scene);

        auto viewer = scene->AddChild(new soil::stage::scene::viewer::Ortho(GetResources().GetWindow()->GetSize()));
        viewer->SetOrthoType(soil::stage::scene::viewer::OrthoType::OrthoHeight);

        // Quadtree is just the data structure
        auto* quadTree = new soil::world::volume::QuadTree(128, 4, 1.0F);
        // The container inserts and updates volume components added to scene
        container_ = scene->AddHook(new soil::stage::scene::volume::Container(quadTree));
        // Frustum culling uses the data of the container for culling based on culling
        // scene->AddComponentFeature(new soil::stage::scene::volume::FrustumCulling(viewer, quadTree));

        const std::vector textures = {GetResources().GetTexture2D(asset::GetPath("Textures/soil.png")),
                                      GetResources().GetTexture2D(asset::GetPath("Textures/carrot.png"))};

        auto* shader = dynamic_cast<basic::Shader*>(GetResources().GetShader(basic::Shader::NAME));
        shader->SetViewer(viewer); // will update PV matrix in Shader::Prepare())
        // shader->SetTextures(textures);

        initBackground(scene, 0);
        initCarrots(scene, 1);
        soil::stage::Stage::Load();
    }

    void Stage::initInput(soil::stage::scene::Scene* scene) {
        auto* inputNode = scene->AddChild(new soil::stage::scene::Input());
        inputNode->GetEventMap()
            .AddKeyMapping(soil::input::Keys::Escape, soil::input::Event::State::Release,
                           [this](const soil::input::Event&) { GetResources().GetWindow()->Close(); })
            .AddKeyMapping(soil::input::Keys::Key_1, soil::input::Event::State::Release,
                           [this](const soil::input::Event&) { shapes_[0]->SetOpaque(!shapes_[0]->IsOpaque()); })
            .AddKeyMapping(soil::input::Keys::Key_2, soil::input::Event::State::Release,
                           [this](const soil::input::Event&) { shapes_[1]->SetOpaque(!shapes_[1]->IsOpaque()); })
            .AddKeyMapping(soil::input::Keys::Key_3, soil::input::Event::State::Release,
                           [this](const soil::input::Event&) { shapes_[2]->SetOpaque(!shapes_[2]->IsOpaque()); })
            .AddKeyMapping(soil::input::Keys::Key_4, soil::input::Event::State::Release,
                           [this](const soil::input::Event&) { shapes_[3]->SetOpaque(!shapes_[3]->IsOpaque()); })
            .AddKeyMapping(soil::input::Keys::S, soil::input::Event::State::Release,
                           [this](const soil::input::Event&) { printStatistics_ = !printStatistics_; });
    }

    void Stage::initBackground(soil::stage::scene::Scene* scene, byte textureUnit) const {
        auto* shader = dynamic_cast<basic::Shader*>(GetResources().GetShader(basic::Shader::NAME));
        const auto* mesh = GetResources().GetMesh({
            .Identifier = "Quad",
        });

        auto bgNode = scene->AddChild(new soil::stage::scene::Node(soil::stage::scene::Node::Type::Visual));
        auto* bgShape = bgNode->AddComponent(new basic::Shape(*mesh, true, shader));
        bgShape->SetSize({10.F, 10.F});
        bgShape->SetTextureUnit(textureUnit);
        bgNode->SetPosition({0.F, 0.F, -1.F});
    }

    void Stage::initCarrots(soil::stage::scene::Scene* scene, byte textureUnit) {
        auto* shader = dynamic_cast<basic::Shader*>(GetResources().GetShader(basic::Shader::NAME));
        const auto* mesh = GetResources().GetMesh({
            .Identifier = "Quad",
        });
        float initRotation = 0.0f;
        constexpr std::array colors = {glm::vec3(1.F, 1.F, 1.F), glm::vec3(0.5F, 1.F, 1.F), glm::vec3(1.F, 0.5F, 1.F),
                                       glm::vec3(1.F, 1.F, 0.5F)};
        for (auto i = 0; i < 4; ++i) {
            auto* shapeNode = scene->AddChild(new common::RotationNode(initRotation));
            shapes_[i] = shapeNode->AddComponent(new basic::Shape(*mesh, true, shader));
            shapes_[i]->SetTextureUnit(textureUnit);
            shapes_[i]->SetColor(glm::vec4(colors[i], 0.6F));
            shapes_[i]->SetSize(glm::vec2(1, 1));
            const auto col = i % 2;
            const auto row = i / 2;
            shapeNode->SetPosition(glm::vec3(col + 0.5 * col, row + 0.5 * row, 0));
            initRotation += 45.F;
            // Add volume
            shapeNode->AddComponent(
                new soil::stage::scene::component::BoundingVolume(new soil::world::volume::AABB(glm::vec3(1, 1, 0))));
        }
    }

    void Stage::Handle(const soil::WindowEvent& event) {
        soil::stage::Stage::Handle(event);
        if (printStatistics_ && event.GetCause() == soil::WindowEvent::StatisticsChanged) {
            const auto stats = event.GetWindow()->GetStatistics();
            PLOG_DEBUG << "FPS: " << std::to_string(stats.FPS) << " Draws: " << std::to_string(stats.DrawCount)
                       << " Vertices: " << std::to_string(stats.VertexCount);
        }
    }
} // namespace soil_samples::volume
