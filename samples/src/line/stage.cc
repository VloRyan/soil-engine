#include "stage.h"

#include <asset.h>
#include <plog/Log.h>

#include <string>

#include "common/rotation_node.h"
#include "glm/glm.hpp"
#include "line_instance.h"
#include "shader.h"
#include "stage/scene/input.h"
#include "stage/scene/scene.h"
#include "stage/scene/viewer/ortho.h"
#include "stage/stage.h"

namespace soil_samples::line {

Stage::Stage() : lines_(), printStatistics_(false), offset_(0) {}

void Stage::OnLoad() {
  auto* scene = AddScene(new soil::stage::scene::Scene());
  scene->SetPipeline(soil::video::render::Pipeline::NewForwardRenderingPipeline(
      scene->GetRenderContainer()));

  const auto viewer = scene->AddChild(new soil::stage::scene::viewer::Ortho(
      GetResources().GetWindow()->GetSize()));
  viewer->Look(glm::vec3(0.F), glm::vec3(0.F, 0.F, -1.F));

  auto* shader = dynamic_cast<Shader*>(GetResources().GetShader(Shader::NAME));
  shader->SetViewer(viewer);  // will update PV matrix in Shader::Prepare())

  auto* mesh =
      GetResources().GetMesh({.Type = soil::video::mesh::Prefab::Type::Line});
  auto* instancing = scene->AddHook(
      new soil::stage::scene::render::Instancing(scene->GetRenderContainer()));
  instancing->AddRenderBatch(
      LineInstance::BATCH_NAME,
      {.Mesh = mesh,
       .Shader = shader,
       .VertexAttribDescriptors = LineInstance::ATTRIBS});

  glLineWidth(1.F);
  initLines(scene);
}

void Stage::Update() {
  /* if (lines_[0] != nullptr) {
       auto inner = 90.0F;
       auto outer = 180.0F;
       offset_++;
       for (auto i = 0; i < MAX_LINES; ++i) {
           lines_[i]->SetStartPoint(glm::vec3(inner * cos(static_cast<float>(i +
   offset_)), inner * sin(static_cast<float>(i + offset_)), -0.1));
           lines_[i]->SetEndPoint(glm::vec3(outer * cos(static_cast<float>(i +
   offset_)), outer * sin(static_cast<float>(i + offset_)), -0.1));
       }
       if (offset_ >= 360) {
           offset_ = 0;
       }
   }*/
  soil::stage::Stage::Update();
}

void Stage::Handle(const soil::WindowEvent& event) {
  soil::stage::Stage::Handle(event);
  if (event.GetCause() == soil::WindowEvent::StatisticsChanged) {
    if (lines_[0] != nullptr) {
      auto inner = 90.0F;
      auto outer = 180.0F;
      offset_++;
      for (auto i = 0; i < MAX_LINES; ++i) {
        /*  lines_[i]->SetStartPoint(glm::vec3(inner * cos(static_cast<float>(i
          - offset_)), inner * sin(static_cast<float>(i - offset_)), -0.1));
          lines_[i]->SetEndPoint(glm::vec3(outer * cos(static_cast<float>(i -
          offset_)), outer * sin(static_cast<float>(i - offset_)), -0.1));*/

        lines_[i]->SetVisible(i <= offset_);
      }
      if (offset_ >= MAX_LINES) {
        offset_ = 0;
      }
    }
    if (printStatistics_) {
      const auto stats = event.GetWindow()->GetStatistics();
      PLOG_DEBUG << "FPS: " << std::to_string(stats.FPS)
                 << " Draws: " << std::to_string(stats.DrawCount / stats.FPS)
                 << " Vertices: "
                 << std::to_string(stats.VertexCount / stats.FPS)
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
}

void Stage::RegisterInputEvents(soil::input::EventMap& eventMap) {
  eventMap.AddKeyMapping(soil::input::Keys::S,
                         soil::input::Event::State::Release,
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
