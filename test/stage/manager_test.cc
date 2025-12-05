#include "stage/manager.h"

#include "gtest/gtest.h"
#include "mocks.hpp"
#include "stage/stage.h"

namespace soil::stage {

class ManagerTest : public testing::Test {};

TEST_F(ManagerTest, Contruct) {
  auto resourcesMock = ResourcesMock();
  auto manager = Manager(resourcesMock);
}

TEST_F(ManagerTest, RegisterAndRemoveStage) {
  auto resourcesMock = ResourcesMock();
  auto manager = Manager(resourcesMock);
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
  auto manager = Manager(resourcesMock);
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
  auto manager = Manager(resourcesMock);
  auto* stage = new StageMock();
  manager.RegisterStage("stage", stage);

  manager.Update();
  EXPECT_EQ(stage->Calls.Update, 0);

  manager.SetCurrent("stage");
  manager.Update();
  EXPECT_EQ(stage->Calls.Update, 1);
}

TEST_F(ManagerTest, Render) {
  auto resourcesMock = ResourcesMock();
  auto manager = Manager(resourcesMock);
  auto* stage = new StageMock();
  auto emptyContext = video::EmptyContext();
  auto state = video::render::State(emptyContext);
  manager.RegisterStage("stage", stage);

  manager.Render(state);
  EXPECT_EQ(stage->Calls.Render, 0);

  manager.SetCurrent("stage");
  manager.Render(state);
  EXPECT_EQ(stage->Calls.Render, 1);
}
}  // namespace soil::stage
