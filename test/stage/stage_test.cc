#include "stage/stage.h"

#include <gmock/gmock-matchers.h>

#include "gtest/gtest.h"
#include "stage/hook/mocks.hpp"
#include "stage/scene/scene.h"

namespace soil::stage {
class StageTest : public testing::Test {};

TEST_F(StageTest, Contruct) {
  const auto stage = Stage();

  EXPECT_EQ(stage.IsLoaded(), false);
  EXPECT_THROW(stage.GetResources(), StageNotRegisteredException);
}

TEST_F(StageTest, AddRemoveUpdateTriggerHooks) {
  auto stage = Stage();
  auto root = stage.AddScene(new scene::Scene());
  auto otherRoot = stage.AddScene(new scene::Scene());  // no triggers
  auto hook = hook::HookMock();
  auto globalBeforeUpdateScenePoint =
      soil::stage::hook::TriggerHook::TriggerPoint{
          .TriggerType =
              soil::stage::hook::TriggerHook::TriggerType::BeforeUpdateScene};
  auto globalAfterUpdateScenePoint =
      soil::stage::hook::TriggerHook::TriggerPoint{
          .TriggerType =
              soil::stage::hook::TriggerHook::TriggerType::AfterUpdateScene};
  auto rootBeforeUpdateScenePoint =
      soil::stage::hook::TriggerHook::TriggerPoint{
          .Root = root,
          .TriggerType =
              soil::stage::hook::TriggerHook::TriggerType::BeforeUpdateScene};
  auto rootAfterUpdateScenePoint = soil::stage::hook::TriggerHook::TriggerPoint{
      .Root = root,
      .TriggerType =
          soil::stage::hook::TriggerHook::TriggerType::AfterUpdateScene};

  stage.AddTriggerHook(&hook, globalBeforeUpdateScenePoint);
  stage.Update();
  EXPECT_THAT(hook.TriggerReceived,
              testing::ElementsAre(globalBeforeUpdateScenePoint));
  hook.Reset();

  stage.AddTriggerHook(&hook, rootBeforeUpdateScenePoint);
  stage.Update();
  EXPECT_THAT(hook.TriggerReceived,
              testing::ElementsAre(globalBeforeUpdateScenePoint,
                                   rootBeforeUpdateScenePoint));
  hook.Reset();

  stage.AddTriggerHook(&hook, rootAfterUpdateScenePoint);
  stage.Update();
  EXPECT_THAT(hook.TriggerReceived,
              testing::ElementsAre(globalBeforeUpdateScenePoint,
                                   rootBeforeUpdateScenePoint,
                                   rootAfterUpdateScenePoint));
  hook.Reset();

  stage.AddTriggerHook(&hook, globalAfterUpdateScenePoint);
  stage.Update();
  EXPECT_THAT(hook.TriggerReceived,
              testing::ElementsAre(
                  globalBeforeUpdateScenePoint, rootBeforeUpdateScenePoint,
                  rootAfterUpdateScenePoint, globalAfterUpdateScenePoint));
  hook.Reset();

  stage.RemoveTriggerHook(&hook, globalBeforeUpdateScenePoint);
  stage.Update();
  EXPECT_THAT(hook.TriggerReceived,
              testing::ElementsAre(rootBeforeUpdateScenePoint,
                                   rootAfterUpdateScenePoint,
                                   globalAfterUpdateScenePoint));
  hook.Reset();

  stage.RemoveTriggerHook(&hook, rootAfterUpdateScenePoint);
  stage.Update();
  EXPECT_THAT(hook.TriggerReceived,
              testing::ElementsAre(rootBeforeUpdateScenePoint,
                                   globalAfterUpdateScenePoint));
  hook.Reset();

  stage.RemoveTriggerHook(&hook, globalAfterUpdateScenePoint);
  stage.Update();
  EXPECT_THAT(hook.TriggerReceived,
              testing::ElementsAre(rootBeforeUpdateScenePoint));
  hook.Reset();

  stage.RemoveTriggerHook(&hook, rootBeforeUpdateScenePoint);
  stage.Update();
  EXPECT_TRUE(hook.TriggerReceived.empty());
}

TEST_F(StageTest, AddRemoveRenderTriggerHooks) {
  auto stage = Stage();
  auto root = stage.AddScene(new scene::Scene());
  auto otherRoot = stage.AddScene(new scene::Scene());  // no triggers
  auto hook = hook::HookMock();
  auto globalBeforeRenderPoint = soil::stage::hook::TriggerHook::TriggerPoint{
      .TriggerType = soil::stage::hook::TriggerHook::TriggerType::BeforeRender};
  auto rootBeforeRenderPoint = soil::stage::hook::TriggerHook::TriggerPoint{
      .Root = root,
      .TriggerType = soil::stage::hook::TriggerHook::TriggerType::BeforeRender};
  auto state = video::render::State{};

  stage.AddTriggerHook(&hook, globalBeforeRenderPoint);
  stage.Render(state);
  EXPECT_THAT(hook.TriggerReceived,
              testing::ElementsAre(globalBeforeRenderPoint));
  hook.Reset();

  stage.AddTriggerHook(&hook, rootBeforeRenderPoint);
  stage.Render(state);
  EXPECT_THAT(
      hook.TriggerReceived,
      testing::ElementsAre(globalBeforeRenderPoint, rootBeforeRenderPoint));
  hook.Reset();

  stage.RemoveTriggerHook(&hook, rootBeforeRenderPoint);
  stage.Render(state);
  EXPECT_THAT(hook.TriggerReceived,
              testing::ElementsAre(globalBeforeRenderPoint));
  hook.Reset();

  stage.RemoveTriggerHook(&hook, globalBeforeRenderPoint);
  stage.Render(state);
  EXPECT_TRUE(hook.TriggerReceived.empty());
}

TEST_F(StageTest, RemoveTriggerHookOnTrigger) {
  auto stage = Stage();
  auto root = stage.AddScene(new scene::Scene());
  auto hook = hook::HookMock();
  auto hook2 = hook::HookMock();
  auto hookPoint = soil::stage::hook::TriggerHook::TriggerPoint{
      .TriggerType =
          soil::stage::hook::TriggerHook::TriggerType::BeforeUpdateScene};

  hook.OnTriggerFunc = [&stage, &hook2, &hookPoint](auto point) {
    stage.RemoveTriggerHook(&hook2, hookPoint);
  };

  stage.AddTriggerHook(&hook, hookPoint);
  stage.AddTriggerHook(&hook2, hookPoint);
  stage.Update();

  EXPECT_THAT(hook.TriggerReceived, testing::ElementsAre(hookPoint));
  EXPECT_TRUE(hook2.TriggerReceived.empty());
}

TEST_F(StageTest, AddRemoveNodeEventHook) {
  auto stage = Stage();
  auto root = stage.AddScene(new scene::Scene());
  auto otherRoot = stage.AddScene(new scene::Scene());  // no triggers
  auto hook = hook::HookMock();
  auto rootHook = hook::HookMock();
  auto nodeEvent = event::Node(root, event::Node::ChangeType::State);

  stage.AddEventHook(nullptr, &hook);
  stage.Handle(nodeEvent);
  EXPECT_THAT(hook.NodeEventsReceived, testing::ElementsAre(nodeEvent));
  hook.Reset();

  stage.AddEventHook(root, &rootHook);
  stage.Handle(nodeEvent);
  EXPECT_THAT(hook.NodeEventsReceived, testing::ElementsAre(nodeEvent));
  EXPECT_THAT(rootHook.NodeEventsReceived, testing::ElementsAre(nodeEvent));
  hook.Reset();
  rootHook.Reset();

  stage.RemoveEventHook(root, &hook);
  stage.Handle(nodeEvent);
  EXPECT_THAT(hook.NodeEventsReceived, testing::ElementsAre(nodeEvent));
  EXPECT_THAT(rootHook.NodeEventsReceived, testing::ElementsAre(nodeEvent));
  hook.Reset();
  rootHook.Reset();

  stage.RemoveEventHook(nullptr, &hook);
  stage.Handle(nodeEvent);
  EXPECT_TRUE(hook.NodeEventsReceived.empty());
  EXPECT_THAT(rootHook.NodeEventsReceived, testing::ElementsAre(nodeEvent));
  hook.Reset();
  rootHook.Reset();

  stage.RemoveEventHook(nullptr, &rootHook);
  stage.Handle(nodeEvent);
  EXPECT_TRUE(hook.NodeEventsReceived.empty());
  EXPECT_THAT(rootHook.NodeEventsReceived, testing::ElementsAre(nodeEvent));
  hook.Reset();
  rootHook.Reset();

  stage.RemoveEventHook(root, &rootHook);
  stage.Handle(nodeEvent);
  EXPECT_TRUE(hook.NodeEventsReceived.empty());
  EXPECT_TRUE(rootHook.NodeEventsReceived.empty());
  hook.Reset();
  rootHook.Reset();
}

TEST_F(StageTest, AddRemoveWindowEventHook) {
  auto stage = Stage();
  auto hook = hook::HookMock();
  auto windowEvent =
      WindowEvent(nullptr, WindowEvent::CauseType::StatisticsChanged);

  stage.AddEventHook(static_cast<hook::EventHook<WindowEvent>*>(&hook));
  stage.Handle(windowEvent);
  EXPECT_THAT(hook.WindowEventsReceived, testing::ElementsAre(windowEvent));
  hook.Reset();

  stage.RemoveEventHook(static_cast<hook::EventHook<WindowEvent>*>(&hook));
  stage.Handle(windowEvent);
  EXPECT_TRUE(hook.WindowEventsReceived.empty());
}

TEST_F(StageTest, AddRemoveInputEventHook) {
  auto stage = Stage();
  auto hook = hook::HookMock();
  auto inputEvent = input::Event::MakeKeyChangedEvent(
      input::Keys::Delete, input::Event::StateType::Press);

  stage.AddEventHook(static_cast<hook::EventHook<input::Event>*>(&hook));
  stage.Handle(inputEvent);
  EXPECT_THAT(hook.InputEventsReceived, testing::ElementsAre(inputEvent));
  hook.Reset();

  stage.RemoveEventHook(static_cast<hook::EventHook<input::Event>*>(&hook));
  stage.Handle(inputEvent);
  EXPECT_TRUE(hook.InputEventsReceived.empty());
}
class DummyGameEvent : public event::GameEvent {
 public:
  DummyGameEvent() = default;
  ~DummyGameEvent() = default;
};
TEST_F(StageTest, AddRemoveGameEventHook) {
  auto stage = Stage();
  auto hook = hook::HookMock();
  auto gameEvent = DummyGameEvent();

  stage.AddEventHook(static_cast<hook::EventHook<event::GameEvent>*>(&hook));
  stage.Handle(gameEvent);
  EXPECT_THAT(hook.GameEventsReceived, testing::ElementsAre(gameEvent));
  hook.Reset();

  stage.RemoveEventHook(static_cast<hook::EventHook<event::GameEvent>*>(&hook));
  stage.Handle(gameEvent);
  EXPECT_TRUE(hook.GameEventsReceived.empty());
}
}  // namespace soil::stage
