#include "stage.h"

#include <asset.h>
#include <plog/Log.h>

#include <string>

#include "common/rotation_node.h"
#include "glm/glm.hpp"
#include "stage/scene/scene.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"
#include "world/volume/aabb.h"
#include "world/volume/quad_tree.h"

namespace soil_samples::world {
Stage::Stage() : shapes_(), world_(nullptr), colObj_(nullptr) {}

void Stage::Update() {
  if (colObj_ != nullptr &&
      colObj_->Object()->GetState() ==
          soil::world::entity::CollisionObject::State::Active) {
    auto velo = colObj_->Object()->GetVelocity();
    auto pos = colObj_->Object()->GetPosition();
    auto diff = std::abs(-4.5F - pos.y);
    if (diff > 0.F) {
      velo.y = std::max(std::max(velo.y + -0.02F, -1.F), -diff);
    } else {
      velo.y = 0.F;
    }
    colObj_->Object()->SetVelocity(velo);
  }
  soil::stage::Stage::Update();
}

soil::stage::scene::viewer::Node* Stage::NewViewer(glm::ivec2 windowSize) {
  const auto viewer = new soil::stage::scene::viewer::Ortho(windowSize);
  viewer->SetOrthoType(soil::stage::scene::viewer::OrthoType::OrthoHeight);
  return viewer;
}

void Stage::OnLoad(soil::stage::scene::Scene* scene) {
  const std::vector textures = {GetResources().Textures().GetTexture2D(
      asset::GetPath("Textures/soil_engine.png")),
                                GetResources().Textures().GetTexture2D(
                                    asset::GetPath("Textures/carrot.png"))};

  auto& renderState = GetResources().GetRenderState();
  for (auto* texture : textures) {
    renderState.SetTexture(
        *texture);  // texture will be bound to next free slot
  }

  auto* shader = GetResources().GetShader(common::component::Shape::SHADER_NAME);
  renderState.SetShader(shader);

  world_ = scene->AddComponent(
      new soil::stage::scene::component::world::WorldComponent(
          new soil::world::World(
              {
                  .Friction = 0.01F,
              },
              new soil::world::volume::QuadTree(64.F))));

  auto* quadVao = GetResources().GetVao("quad");
  common::component::Shape::PREFABS.Insert("shapeDefault",
                                           {
                                               .QuadVao = quadVao,
                                               .Shader = shader,
                                           });

  initBackground(scene, textures[0]->GetSlot());
  initCarrots(scene, textures[1]->GetSlot());
}

void Stage::RegisterInputEvents(soil::input::EventMap& eventMap) {
  eventMap
      .AddKeyMapping(soil::input::Keys::Key_1,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       colObj_->SetPosition(glm::vec3(0.F, 4.5F, -0.1F));
                       colObj_->SetVelocity(glm::vec3(0.F, -0.0001F, 0.F));
                     })
      .AddKeyMapping(soil::input::Keys::S,
                     soil::input::Event::StateType::Release,
                     [this](const soil::input::Event&) {
                       printStatistics_ = !printStatistics_;
                     });
}

void Stage::initBackground(soil::stage::scene::Scene* scene,
                           const byte textureUnit) const {
  const auto bgNode = scene->AddChild(
      new soil::stage::scene::Node(soil::stage::scene::Node::Type::Visual));
  auto* bgShape = bgNode->AddComponent(new common::component::Shape("shapeDefault"));
  bgShape->SetSize({10.F, 10.F});
  bgShape->SetTextureUnit(textureUnit);
  bgNode->SetPosition({0.F, 0.F, -1.F});
}

void Stage::initCarrots(soil::stage::scene::Scene* scene,
                        const byte textureUnit) {
  constexpr std::array colors = {
      glm::vec3(1.F, 1.F, 1.F), glm::vec3(0.5F, 1.F, 1.F),
      glm::vec3(1.F, 0.5F, 1.F), glm::vec3(1.F, 1.F, 0.5F)};
  float initRotation = 0.0f;
  auto* shapeNode = scene->AddChild(new common::RotationNode(initRotation));
  shapes_[0] = shapeNode->AddComponent(new common::component::Shape("shapeDefault"));
  shapes_[0]->SetTextureUnit(textureUnit);
  shapes_[0]->SetSize(glm::vec2(1, 1));
  shapes_[0]->SetColor(glm::vec4(colors[0], 1.0F));
  shapeNode->SetPosition(glm::vec3(0.1F, 4.5F, -0.1F));
  auto* obj =
      new soil::stage::scene::component::world::CollisionObjectComponent(
          new soil::world::volume::AABB(glm::vec3(1.F)),
          soil::world::entity::CollisionObject::ContactType::Object);
  colObj_ = shapeNode->AddComponent(obj);
  world_->Activate(colObj_);
}

}  // namespace soil_samples::world
