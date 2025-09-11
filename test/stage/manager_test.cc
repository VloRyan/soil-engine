#include "stage/manager.h"

#include "gtest/gtest.h"
#include "mocks.hpp"
#include "stage/stage.h"

namespace soil::stage {
    class ManagerTest : public testing::Test {};

    TEST_F(ManagerTest, Contruct) {
        auto manager = Manager();
    }

    TEST_F(ManagerTest, RegisterAndRemoveStage) {
        auto manager = Manager();
        auto stage = Stage();

        manager.RegisterStage("stage", &stage);

        EXPECT_EQ(manager.GetStage("stage"), &stage);
        EXPECT_EQ(manager.GetStage("stage1"), nullptr);

        EXPECT_EQ(manager.RemoveStage("stage"), &stage);

        EXPECT_EQ(manager.RemoveStage("stage1"), nullptr);
        EXPECT_EQ(manager.GetStage("stage"), nullptr);
    }

    TEST_F(ManagerTest, SetCurrent) {
        auto manager = Manager();
        auto stage = Stage();
        manager.RegisterStage("stage", &stage);

        manager.SetCurrent("stage");
        EXPECT_EQ(manager.GetCurrent(), &stage);

        EXPECT_THROW(manager.SetCurrent("stage1"), std::runtime_error);
        EXPECT_EQ(manager.GetCurrent(), &stage);

        manager.RemoveStage("stage");
        EXPECT_EQ(manager.GetCurrent(), nullptr);
    }

    TEST_F(ManagerTest, Update) {
        auto manager = Manager();
        auto* stage = new StageMock();
        manager.RegisterStage("stage", stage);

        manager.Update();
        EXPECT_EQ(stage->UpdateCalledCount, 0);

        manager.SetCurrent("stage");
        manager.Update();
        EXPECT_EQ(stage->UpdateCalledCount, 1);
    }

    TEST_F(ManagerTest, Render) {
        auto manager = Manager();
        auto* stage = new StageMock();
        auto state = video::render::State();
        manager.RegisterStage("stage", stage);

        manager.Render(state);
        EXPECT_EQ(stage->RenderCalledCount, 0);

        manager.SetCurrent("stage");
        manager.Render(state);
        EXPECT_EQ(stage->RenderCalledCount, 1);
    }
} // namespace soil::stage
