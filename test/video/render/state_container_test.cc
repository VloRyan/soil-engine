
#include "video/render/state_container.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "mocks.hpp"
namespace soil::video::render {

class StateContainerTest : public testing::Test {
 protected:
};

TEST_F(StateContainerTest, Insert) {
  auto container = StateContainer();
  auto r1 = RenderableMock({});

  container.Insert(&r1);

  EXPECT_THAT(container.States(), testing::ElementsAre(r1.StateId()));
  EXPECT_THAT(container.GroupByState(r1.StateId())->Content(),
              testing::ElementsAre(&r1));
}

TEST_F(StateContainerTest, InsertSorted) {
  auto container =
      StateContainer([](const StateIdentifier& a, const StateIdentifier& b) {
        if (a.State.Blend != b.State.Blend) {
          return a.State.Blend.value_or(false);
        }
        if (a.State.DepthTest != b.State.DepthTest) {
          return a.State.DepthTest.value_or(false);
        }
        return false;
      });
  auto r1 = RenderableMock({.State = {.Blend = false, .DepthTest = false}});
  auto r2 = RenderableMock({.State = {.Blend = true, .DepthTest = true}});
  auto r3 = RenderableMock({.State = {.Blend = true, .DepthTest = false}});

  container.Insert(&r1);
  container.Insert(&r2);
  container.Insert(&r3);

  EXPECT_THAT(container.States(),
              testing::ElementsAre(r2.StateId(), r3.StateId(), r1.StateId()));
}

TEST_F(StateContainerTest, Remove) {
  auto container = StateContainer();
  auto r1 = RenderableMock({.VAOId = 1, .DrawMode = DrawMode::Points});
  auto r2 = RenderableMock({.VAOId = 2, .DrawMode = DrawMode::Points});
  auto r3 = RenderableMock({.VAOId = 2, .DrawMode = DrawMode::Lines});
  container.Insert(&r1);
  container.Insert(&r2);
  container.Insert(&r3);
  ASSERT_THAT(container.GroupByState(r1.StateId())->Content(),
              testing::ElementsAre(&r1));
  ASSERT_THAT(container.GroupByState(r2.StateId())->Content(),
              testing::ElementsAre(&r2));
  ASSERT_THAT(container.GroupByState(r3.StateId())->Content(),
              testing::ElementsAre(&r3));
  EXPECT_THAT(container.States(),
              testing::ElementsAre(r1.StateId(), r2.StateId(), r3.StateId()));

  container.Remove(&r3);
  EXPECT_THAT(container.GroupByState(r1.StateId())->Content(),
              testing::ElementsAre(&r1));
  EXPECT_THAT(container.GroupByState(r2.StateId())->Content(),
              testing::ElementsAre(&r2));
  EXPECT_TRUE(container.GroupByState(r3.StateId()) == nullptr);
  EXPECT_THAT(container.States(),
              testing::ElementsAre(r1.StateId(), r2.StateId()));

  container.Remove(&r2);
  EXPECT_THAT(container.GroupByState(r1.StateId())->Content(),
              testing::ElementsAre(&r1));
  EXPECT_TRUE(container.GroupByState(r2.StateId()) == nullptr);
  EXPECT_TRUE(container.GroupByState(r3.StateId()) == nullptr);
  EXPECT_THAT(container.States(), testing::ElementsAre(r1.StateId()));

  container.Remove(&r1);
  EXPECT_TRUE(container.GroupByState(r1.StateId()) == nullptr);
  EXPECT_TRUE(container.GroupByState(r2.StateId()) == nullptr);
  EXPECT_TRUE(container.GroupByState(r3.StateId()) == nullptr);
  EXPECT_TRUE(container.States().empty());
}
}  // namespace soil::video::render
