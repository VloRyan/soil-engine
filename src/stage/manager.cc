#include "stage/manager.h"

#include "input/manager.h"
#include "stage/stage.h"
#include "window.h"

#include "stage/scene/scene.h"

namespace stage {
Manager::Manager(video::Manager *videoManager, sound::Manager *soundManager,
                 input::Manager *inputManager)
    : currentStage_(-1), videoManager_(videoManager),
      soundManager_(soundManager), inputManager_(inputManager),
      window_(nullptr), nextStageId_(0) {};

Manager::~Manager() {
  if (window_ != nullptr) {
    window_->RemoveListener(this);
  }
  inputManager_->RemoveListener(this);
  for (const auto *stage : stages_) {
    delete stage;
  }
}

int Manager::AddStage(Stage *stage) {
  stages_.emplace_back(stage);
  if (stages_.size() == 1) {
    currentStage_ = 0;
  }
  return ++nextStageId_;
}

void Manager::Init(engine::Window *window) {
  window_ = window;
  window_->AddListener(this);
  inputManager_->AddListener(this);
}

void Manager::Update() const {
#ifdef DEBUG
  if (currentStage_ == -1) {
    throw std::runtime_error("currentStage_ not set");
  }
#endif
  stages_[currentStage_]->Update();
}

void Manager::Render(video::render::State &state) const {
#ifdef DEBUG
  if (currentStage_ == -1) {
    throw std::runtime_error("currentStage_ not set");
  }
#endif
  stages_[currentStage_]->Render(state);
}

void Manager::Handle(const input::Event &event) {
#ifdef DEBUG
  if (currentStage_ == -1) {
    throw std::runtime_error("currentStage_ not set");
  }
#endif
  stages_[currentStage_]->Handle(event);
}

void Manager::Handle(const engine::WindowEvent &event) {
#ifdef DEBUG
  if (currentStage_ == -1) {
    throw std::runtime_error("currentStage_ not set");
  }
#endif
  stages_[currentStage_]->Handle(event);
}

video::mesh::Mesh *
Manager::GetMesh(const video::mesh::Definition &definition) const {
  return videoManager_->GetMesh(definition);
}

video::shader::InstanceShader *
Manager::GetShader(const std::string &name) const {
  return videoManager_->GetIShader(name);
}

sound::Source *Manager::GetSource(const std::string &name) const {
  return soundManager_->GetSource(name);
}

sound::Buffer *Manager::GetSoundBuffer(const std::string &name) const {
  return soundManager_->GetBuffer(name);
}

sound::Listener *Manager::GetListener() const {
  return soundManager_->GetListener();
}

video::texture::Texture *
Manager::GetTexture2D(const std::string &fileName,
                      const video::texture::Parameter &parameter) {
  return videoManager_->Texture().GetTexture2D(fileName, parameter);
}

video::texture::Texture *
Manager::GetTextureArray2D(const std::string &fileName, int tilesPerDim,
                           const video::texture::Parameter &parameter) {
  return videoManager_->Texture().GetTextureArray2D(fileName, tilesPerDim,
                                                    parameter);
}

engine::Window *Manager::GetWindow() const { return window_; }
} // namespace stage
