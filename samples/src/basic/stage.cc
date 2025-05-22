#include "stage.h"


#include <string>

#include "glm/glm.hpp"


#include "common/rotation_node.h"
#include "shader.h"
#include "shape.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"

#include <plog/Log.h>
#include <stage/scene/input.h>

#include "stage/scene/render/plain.h"

namespace soil_samples::basic {
    Stage::Stage(soil::stage::Manager *manager) : soil::stage::Stage(manager), printStatistics_(false) {
        auto *scene = new soil::stage::scene::Scene(this);
        scene->AddComponentFeature(new soil::stage::scene::render::Plain());

        initInput(scene);

        auto viewer = new soil::stage::scene::viewer::Ortho(scene, GetResources().GetWindow()->GetSize());
        viewer->SetOrthoType(soil::stage::scene::viewer::OrthoType::OrthoHeight);

        auto *shader = dynamic_cast<Shader *>(GetResources().GetShader(Shader::NAME));

        shader->Use();
        shader->SetViewer(viewer); // will update PV matrix in Shader::Prepare())

        initBackground(scene, "Textures/soil.png");
        initCarrots(scene, "Textures/carrot.png");
    }

    void Stage::initInput(soil::stage::scene::Scene *scene) {
        auto *inputNode = new soil::stage::scene::Input(scene);
        inputNode->GetEventMap()
            .AddKeyMapping(soil::input::Keys::Escape, soil::input::Event::State::Release,
                           [this](const soil::input::Event &) { GetResources().GetWindow()->Close(); })
            .AddKeyMapping(soil::input::Keys::Key_1, soil::input::Event::State::Release,
                           [this](const soil::input::Event &) { shapes_[0]->SetOpaque(!shapes_[0]->IsOpaque()); })
            .AddKeyMapping(soil::input::Keys::Key_2, soil::input::Event::State::Release,
                           [this](const soil::input::Event &) { shapes_[1]->SetOpaque(!shapes_[1]->IsOpaque()); })
            .AddKeyMapping(soil::input::Keys::Key_3, soil::input::Event::State::Release,
                           [this](const soil::input::Event &) { shapes_[2]->SetOpaque(!shapes_[2]->IsOpaque()); })
            .AddKeyMapping(soil::input::Keys::Key_4, soil::input::Event::State::Release,
                           [this](const soil::input::Event &) { shapes_[3]->SetOpaque(!shapes_[3]->IsOpaque()); })
            .AddKeyMapping(soil::input::Keys::S, soil::input::Event::State::Release,
                           [this](const soil::input::Event &) { printStatistics_ = !printStatistics_; });
    }

    void Stage::initBackground(soil::stage::scene::Scene *scene, const std::string &textureName) const {
        auto *shader = dynamic_cast<Shader *>(GetResources().GetShader(Shader::NAME));
        auto *bgTexture = GetResources().GetTexture2D(textureName, {});
        const auto *mesh = GetResources().GetMesh({
            .Identifier = "Quad",
        });

        auto bgNode = new soil::stage::scene::Node(scene, soil::stage::scene::Node::Type::Visual, {});
        auto *bgShape = bgNode->AddComponent(new Shape(*mesh, true, shader));
        bgShape->SetSize({10.F, 10.F});
        bgShape->SetTexture(bgTexture);
        bgNode->SetPosition({0.F, 0.F, -1.F});
    }

    void Stage::initCarrots(soil::stage::scene::Scene *scene, const std::string &textureName) {
        auto *shader = dynamic_cast<Shader *>(GetResources().GetShader(Shader::NAME));
        auto *texture = GetResources().GetTexture2D(textureName, {});
        const auto *mesh = GetResources().GetMesh({
            .Identifier = "Quad",
        });
        float initRotation = 0.0f;
        constexpr std::array colors = {glm::vec3(1.F, 1.F, 1.F), glm::vec3(0.5F, 1.F, 1.F), glm::vec3(1.F, 0.5F, 1.F),
                                       glm::vec3(1.F, 1.F, 0.5F)};
        for (auto i = 0; i < 4; ++i) {
            auto *shapeNode = new common::RotationNode(scene, initRotation);
            shapes_[i] = shapeNode->AddComponent(new Shape(*mesh, true, shader));
            shapes_[i]->SetTexture(texture);
            shapes_[i]->SetColor(glm::vec4(colors[i], 0.6F));
            shapes_[i]->SetSize(glm::vec2(1, 1));
            auto col = i % 2;
            auto row = i / 2;
            shapeNode->SetPosition(glm::vec3(col + 0.5 * col, row + 0.5 * row, 0));
            initRotation += 45.F;
        }
    }

    void Stage::Handle(const soil::WindowEvent &event) {
        soil::stage::Stage::Handle(event);
        if (printStatistics_ && event.GetCause() == soil::WindowEvent::StatisticsChanged) {
            const auto stats = event.GetWindow()->GetStatistics();
            PLOG_DEBUG << "FPS: " << std::to_string(stats.fps) << " Draws: " << std::to_string(stats.drawCount)
                       << " Vertices: " << std::to_string(stats.vertexCount);
        }
    }
} // namespace soil_samples::basic
