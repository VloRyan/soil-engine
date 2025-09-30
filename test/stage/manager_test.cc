#include "stage/manager.h"

#include "gtest/gtest.h"
#include "mocks.hpp"
#include "stage/stage.h"

namespace soil::stage {
class ResourcesMock : public Resources {
 public:
  ResourcesMock() : Resources(nullptr, nullptr, nullptr, nullptr) {}

  ~ResourcesMock() {
    delete TextureManager;
    delete RenderState;
  }

  [[nodiscard]] video::mesh::Data* GetMesh(
      const video::mesh::Prefab::Definition& definition) const {
    return nullptr;
  }

  [[nodiscard]] video::shader::Shader* GetShader(
      const std::string& name) const {
    return nullptr;
  }

  [[nodiscard]] sound::Source* GetSource(const std::string& name) const {
    return nullptr;
  }

  [[nodiscard]] sound::Buffer* GetSoundBuffer(const std::string& name) const {
    return nullptr;
  }

  [[nodiscard]] sound::Listener* GetListener() const { return nullptr; }

  [[nodiscard]] Window* GetWindow() const { return nullptr; }

  [[nodiscard]] video::texture::Manager& Textures() const {
    return *TextureManager;
  }

  [[nodiscard]] video::render::State& GetRenderState() const {
    return *RenderState;
  }

  video::texture::Manager* TextureManager = new video::texture::Manager();

  video::render::State* RenderState = new video::render::State();
};

class ManagerTest : public testing::Test {};

TEST_F(ManagerTest, Contruct) { auto manager = Manager(nullptr); }

TEST_F(ManagerTest, RegisterAndRemoveStage) {
  auto resourcesMock = ResourcesMock();
  auto manager = Manager(&resourcesMock);
  auto stage = Stage();

  manager.RegisterStage("stage", &stage);

  EXPECT_EQ(manager.GetStage("stage"), &stage);
  EXPECT_EQ(manager.GetStage("stage1"), nullptr);

  EXPECT_EQ(manager.RemoveStage("stage"), &stage);

  EXPECT_EQ(manager.RemoveStage("stage1"), nullptr);
  EXPECT_EQ(manager.GetStage("stage"), nullptr);
}

TEST_F(ManagerTest, SetCurrent) {
  auto resourcesMock = ResourcesMock();
  auto manager = Manager(&resourcesMock);
  auto stage = Stage();
  auto otherStage = Stage();
  manager.RegisterStage("stage", &stage);

  manager.SetCurrent("stage");
  EXPECT_EQ(manager.GetCurrent(), &stage);

  EXPECT_THROW(manager.SetCurrent("stage1"), std::runtime_error);
  EXPECT_EQ(manager.GetCurrent(), &stage);

  manager.RegisterStage("other", &otherStage);
  EXPECT_EQ(manager.GetCurrent(), &stage);

  manager.SetCurrent("other");
  EXPECT_EQ(manager.GetCurrent(), &stage);

  manager.Update();
  EXPECT_EQ(manager.GetCurrent(), &otherStage);

  stage.SetCurrent();
  manager.Update();
  EXPECT_EQ(manager.GetCurrent(), &stage);

  manager.RemoveStage("stage");
  manager.RemoveStage("other");
  EXPECT_EQ(manager.GetCurrent(), nullptr);
}

TEST_F(ManagerTest, Update) {
  auto resourcesMock = ResourcesMock();
  auto manager = Manager(&resourcesMock);
  auto* stage = new StageMock();
  manager.RegisterStage("stage", stage);

  manager.Update();
  EXPECT_EQ(stage->UpdateCalledCount, 0);

  manager.SetCurrent("stage");
  manager.Update();
  EXPECT_EQ(stage->UpdateCalledCount, 1);
}

TEST_F(ManagerTest, Render) {
  auto resourcesMock = ResourcesMock();
  auto manager = Manager(&resourcesMock);
  auto* stage = new StageMock();
  auto state = video::render::State();
  manager.RegisterStage("stage", stage);

  manager.Render(state);
  EXPECT_EQ(stage->RenderCalledCount, 0);

  manager.SetCurrent("stage");
  manager.Render(state);
  EXPECT_EQ(stage->RenderCalledCount, 1);
}
}  // namespace soil::stage
