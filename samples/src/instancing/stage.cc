#include "stage.h"
#include <string>


#include "engine.h"
#include "glm/glm.hpp"
#include "shader.h"

#include "common/rotation_node.h"
#include "common/statistic_scene.h"
#include "shape_instance.h"
#include "stage/scene/input.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"

#include <plog/Log.h>

namespace soil_samples::instancing {

    Stage::Stage(soil::stage::Manager *manager) : soil::stage::Stage(manager), printStatistics_(false) {
        auto *scene = new common::StatisticScene(this);

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
                           [this](const soil::input::Event &) { shapes_[3]->SetOpaque(!shapes_[3]->IsOpaque()); });

        auto *viewer = new soil::stage::scene::viewer::Ortho(scene, GetResources().GetWindow()->GetSize());
        viewer->SetOrthoType(soil::stage::scene::viewer::OrthoType::OrthoHeight);

        auto *shader = dynamic_cast<Shader *>(GetResources().GetShader(Shader::NAME));
        std::vector textures = {GetResources().GetTexture2D("Textures/soil.png", {}),
                                GetResources().GetTexture2D("Textures/carrot.png", {})};

        shader->Use();
        shader->SetTextures2d(textures, "Textures");
        shader->SetViewer(viewer); // will update PV matrix in Shader::Prepare())

        auto *mesh = GetResources().GetMesh({.Identifier = "Quad"});
        scene->Instancing()->AddRenderBatch(
            "Shape", {.Mesh = mesh, .Shader = shader, .VertexAttribDescriptors = ShapeInstance::ATTRIBS});


        const auto bgNode = new soil::stage::scene::Node(scene, soil::stage::scene::Node::Type::Visual, {});
        auto *bgShape = bgNode->AddComponent(new ShapeInstance("Shape", true));
        bgShape->SetSize({10.F, 10.F});
        bgShape->SetTextureIndex(0);
        bgNode->SetPosition({0.F, 0.F, -1.F});

        constexpr std::array colors = {glm::vec3(1.F, 1.F, 1.F), glm::vec3(0.5F, 1.F, 1.F), glm::vec3(1.F, 0.5F, 1.F),
                                       glm::vec3(1.F, 1.F, 0.5F)};
        float initRotation = 0.0f;
        for (auto i = 0; i < 4; ++i) {
            auto *shapeNode = new common::RotationNode(scene, initRotation);
            shapes_[i] = shapeNode->AddComponent(new ShapeInstance("Shape", true));
            shapes_[i]->SetTextureIndex(1);
            shapes_[i]->SetSize(glm::vec2(1, 1));
            shapes_[i]->SetColor(glm::vec4(colors[i], 0.6F));
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
} // namespace soil_samples::instancing
