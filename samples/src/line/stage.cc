#include "stage.h"

#include <string>

#include "glm/glm.hpp"
#include "line_instance.h"
#include "stage/scene/scene.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"

namespace soil_samples::line {

Stage::Stage() : lines_(), offset_(0) {}

void Stage::OnLoad(soil::stage::scene::Scene* scene) {
  auto* shader = GetResources().GetShader(LineInstance::SHADER_NAME);
  soil::video::render::draw::VaoElementsInstanced::Prepare({
      .Name = LineInstance::BATCH_NAME,
      .MeshData = GetResources().GetMesh(
          {.Type = soil::video::mesh::Prefab::Type::Line}),
      .Shader = shader,
      .VertexAttribDescriptors = LineInstance::ATTRIBS,
  });
  initLines(scene);
}

void Stage::OnStatsChanges(const soil::Engine::Statistics& stats) {
  common::Stage::OnStatsChanges(stats);
  if (lines_[0] != nullptr) {
    offset_++;
    for (auto i = 0; i < MAX_LINES; ++i) {
      lines_[i]->SetVisible(i <= offset_);
    }
    if (offset_ >= MAX_LINES) {
      offset_ = 0;
    }
  }
}

void Stage::RegisterInputEvents(soil::input::EventMap& eventMap) {
  eventMap.AddKeyMapping(soil::input::Keys::S,
                         soil::input::Event::StateType::Release,
                         [this](const soil::input::Event&) {
                           printStatistics_ = !printStatistics_;
                         });
}

void Stage::initLines(soil::stage::scene::Scene* scene) {
  auto inner = 90.0F;
  auto outer = 180.0F;
  auto part = MAX_LINES / 3.F;
  for (auto i = 0; i < MAX_LINES; ++i) {
    auto* node = scene->AddChild(new soil::stage::scene::Node(
        soil::stage::scene::Node::Type::Transform));
    lines_[i] = node->AddComponent(
        new LineInstance(LineInstance::BATCH_NAME,                         //
                         glm::vec3(inner * cos(i), inner * sin(i), -0.1),  //
                         glm::vec3(outer * cos(i), outer * sin(i), -0.1)));
    lines_[i]->SetVisible(false);
    node->SetPosition(glm::vec3(500, 500, 0));
    auto remain = static_cast<float>(i);
    glm::vec3 color;
    if (remain > part) {
      remain -= part;
      color.r = 1.F;
      color.g = remain / part;
      if (remain > part) {
        remain -= part;
        color.g = 1.F;
        color.b = remain / part;
      }
    } else {
      color.r = remain / part;
    }

    if (i > 0) {
      lines_[i]->SetColor(glm::vec4(color, 1.F));
    }
  }
}
}  // namespace soil_samples::line
