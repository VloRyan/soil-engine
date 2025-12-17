#include "stage.h"

#include <asset.h>
#include <plog/Log.h>

#include <string>

#include "common/rotation_node.h"
#include "engine.h"
#include "glm/glm.hpp"
#include "shader.h"
#include "shape_instance.h"
#include "stage/scene/scene.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"

namespace soil_samples::instancing {

Stage::Stage() : shapes_() {}

void Stage::OnLoad() {
  auto* scene = AddScene(new soil::stage::scene::Scene());

  auto* viewer = scene->AddChild(new soil::stage::scene::viewer::Ortho(
      GetResources().GetWindow()->GetSize()));
  viewer->SetOrthoType(soil::stage::scene::viewer::OrthoType::OrthoHeight);

  auto* shader = dynamic_cast<Shader*>(GetResources().GetShader(Shader::NAME));
  const std::vector textures = {GetResources().Textures().GetTexture2D(
                                    asset::GetPath("Textures/soil_engine.png")),
                                GetResources().Textures().GetTexture2D(
                                    asset::GetPath("Textures/carrot.png"))};
  shader->SetViewer(viewer);  // will update PV matrix in Shader::Prepare())
  shader->SetTextures(textures);
  auto& renderState = GetResources().GetRenderState();
  for (auto* texture : textures) {
    renderState.SetTexture(
        *texture);  // texture will be bound to next free slot
  }

  soil::video::render::draw::VaoElementsInstanced::Prepare({
      .Name = "shape",
      .MeshData = GetResources().GetMesh({.Identifier = "Quad"}),
      .Shader = shader,
      .VertexAttribDescriptors = ShapeInstance::ATTRIBS,
  });

  initBackground(scene, 0);
  initCarrots(scene, 1);
}

void Stage::RegisterInputEvents(soil::input::EventMap& eventMap) {
  eventMap
      .AddKeyMapping(soil::input::Keys::Key_1,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       shapes_[0]->SetVisible(!shapes_[0]->IsVisible());
                     })
      .AddKeyMapping(soil::input::Keys::Key_2,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       if (shapes_[1] != nullptr) {
                         delete shapes_[1];
                         shapes_[1] = nullptr;
                       }
                     })
      .AddKeyMapping(soil::input::Keys::S,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       printStatistics_ = !printStatistics_;
                     });
}

void Stage::initBackground(soil::stage::scene::Scene* scene,
                           const int textureIndex) {
  const auto bgNode = scene->AddChild(
      new soil::stage::scene::Node(soil::stage::scene::Node::Type::Visual));
  auto* bgShape = bgNode->AddComponent(new ShapeInstance("shape"));
  bgShape->SetSize({10.F, 10.F});
  bgShape->SetTextureIndex(textureIndex);
  bgNode->SetPosition({0.F, 0.F, -1.F});
}

void Stage::initCarrots(soil::stage::scene::Scene* scene,
                        const int textureIndex) {
  constexpr std::array colors = {
      glm::vec3(1.F, 1.F, 1.F), glm::vec3(0.5F, 1.F, 1.F),
      glm::vec3(1.F, 0.5F, 1.F), glm::vec3(1.F, 1.F, 0.5F)};
  float initRotation = 0.0f;
  constexpr auto offset = glm::vec2(SHAPES_PER_DIM * -0.5F);
  for (auto row = 0; row < SHAPES_PER_DIM; ++row) {
    for (auto col = 0; col < SHAPES_PER_DIM; ++col) {
      const auto i = row * SHAPES_PER_DIM + col;
      auto* shapeNode = scene->AddChild(new common::RotationNode(initRotation));
      shapes_[i] = shapeNode->AddComponent(new ShapeInstance("shape"));
      shapes_[i]->SetTextureIndex(textureIndex);
      shapes_[i]->SetSize(glm::vec2(1, 1));
      shapes_[i]->SetColor(glm::vec4(colors[i % 4], 1.0F));
      shapeNode->SetPosition(glm::vec3(static_cast<float>(col) + offset.x,
                                       static_cast<float>(row) + offset.y,
                                       -0.5F));
      initRotation += 45.F;
    }
  }
}
}  // namespace soil_samples::instancing
