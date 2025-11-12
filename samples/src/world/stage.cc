#include "stage.h"

#include <asset.h>
#include <plog/Log.h>

#include <string>

#include "basic/shader.h"
#include "basic/shape.h"
#include "common/rotation_node.h"
#include "glm/glm.hpp"
#include "stage/scene/scene.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"
#include "world/volume/aabb.h"
#include "world/volume/quad_tree.h"

namespace soil_samples::world {
Stage::Stage()
    : printStatistics_(false), shapes_(), world_(nullptr), colObj_(nullptr) {}

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
void Stage::OnLoad() {
  auto* scene = AddScene(new soil::stage::scene::Scene());

  const auto viewer = scene->AddChild(new soil::stage::scene::viewer::Ortho(
      GetResources().GetWindow()->GetSize()));
  viewer->SetOrthoType(soil::stage::scene::viewer::OrthoType::OrthoHeight);

  const std::vector textures = {GetResources().Textures().GetTexture2D(
                                    asset::GetPath("Textures/soil_engine.png")),
                                GetResources().Textures().GetTexture2D(
                                    asset::GetPath("Textures/carrot.png"))};

  auto& renderState = GetResources().GetRenderState();
  for (auto* texture : textures) {
    renderState.SetTexture(
        *texture);  // texture will be bound to next free slot
  }

  auto* shader = dynamic_cast<basic::Shader*>(
      GetResources().GetShader(basic::Shader::NAME));
  shader->Use();
  shader->SetViewer(viewer);  // will update PV matrix in Shader::Prepare())

  world_ = scene->AddChild(
      new soil::stage::scene::world::WorldNode(new soil::world::World(
          {
              .Friction = 0.01F,
          },
          new soil::world::volume::QuadTree(64.F))));

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
  auto* shader = dynamic_cast<basic::Shader*>(
      GetResources().GetShader(basic::Shader::NAME));
  const auto* mesh = GetResources().GetMesh({
      .Identifier = "Quad",
  });

  const auto bgNode = scene->AddChild(
      new soil::stage::scene::Node(soil::stage::scene::Node::Type::Visual));
  auto* bgShape = bgNode->AddComponent(new basic::Shape(*mesh, true, shader));
  bgShape->SetSize({10.F, 10.F});
  bgShape->SetTextureUnit(textureUnit);
  bgNode->SetPosition({0.F, 0.F, -1.F});
}

void Stage::initCarrots(soil::stage::scene::Scene* scene,
                        const byte textureUnit) {
  auto* shader = dynamic_cast<basic::Shader*>(
      GetResources().GetShader(basic::Shader::NAME));
  const auto* mesh = GetResources().GetMesh({
      .Identifier = "Quad",
  });
  constexpr std::array colors = {
      glm::vec3(1.F, 1.F, 1.F), glm::vec3(0.5F, 1.F, 1.F),
      glm::vec3(1.F, 0.5F, 1.F), glm::vec3(1.F, 1.F, 0.5F)};
  float initRotation = 0.0f;
  auto* shapeNode = world_->AddChild(new common::RotationNode(initRotation));
  shapes_[0] = shapeNode->AddComponent(new basic::Shape(*mesh, true, shader));
  shapes_[0]->SetTextureUnit(textureUnit);
  shapes_[0]->SetSize(glm::vec2(1, 1));
  shapes_[0]->SetColor(glm::vec4(colors[0], 1.0F));
  shapeNode->SetPosition(glm::vec3(0.1F, 4.5F, -0.1F));
  auto* obj = new soil::stage::scene::component::CollisionObjectComponent(
      new soil::world::volume::AABB(glm::vec3(1.F)),
      soil::world::entity::CollisionObject::ContactType::Object);
  colObj_ = shapeNode->AddComponent(obj);
  world_->Activate(colObj_);
}

void Stage::Handle(const soil::WindowEvent& event) {
  soil::stage::Stage::Handle(event);
  if (printStatistics_ && event.Cause == soil::WindowEvent::StatisticsChanged) {
    const auto stats = event.Window->GetStatistics();
    PLOG_DEBUG << "FPS: " << std::to_string(stats.FPS)
               << " Draws: " << std::to_string(stats.DrawCount / stats.FPS)
               << " Vertices: " << std::to_string(stats.VertexCount / stats.FPS)
               << " State changes: "
               << std::to_string(stats.StateChanges / stats.FPS)
               << " Update times: "
               << std::to_string(stats.updateInputTime / stats.FPS) << ", "
               << std::to_string(stats.updateStageTime / stats.FPS) << ", "
               << std::to_string(stats.updateVideoTime / stats.FPS)
               << " Render times: "
               << std::to_string(stats.startRenderTime / stats.FPS) << ", "
               << std::to_string(stats.renderTime / stats.FPS) << ", "
               << std::to_string(stats.endRenderTime / stats.FPS);
  }
}

}  // namespace soil_samples::world
