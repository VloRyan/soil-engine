#include "stage/scene/component/event/hook.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "stage/mocks.hpp"
#include "stage/scene/node.h"
namespace soil::stage::scene::component::event {
class HookSpy : public Hook {
 public:
  explicit HookSpy(
      const std::vector<EventType> &events = {},
      const std::vector<soil::stage::hook::TriggerHook::TriggerPoint> &points =
          {})
      : Hook(events, points) {}
  soil::stage::Stage *Stage() { return stage_; };
  const std::vector<EventType> &ActiveEvents() { return activeEvents_; };
  const std::vector<soil::stage::hook::TriggerHook::TriggerPoint> &
  ActiveTriggerPoints() {
    return activeTriggerPoints_;
  };
  std::function<void(const stage::event::Node &event)> NodeEventCallback() {
    return nodeEventCallback_;
  };
  std::function<void(const input::Event &event)> InputEventCallback() {
    return inputEventCallback_;
  };
  std::function<void(const WindowEvent &event)> WindowEventCallback() {
    return windowEventCallback_;
  };
  std::function<void(const TriggerPoint &point)> TriggerCallback() {
    return triggerCallback_;
  };
  stage::scene::Node *TriggerRoot() { return triggerRoot_; };
};
class HookTest : public testing::Test {};

TEST_F(HookTest, Construct) {
  auto hook = HookSpy();
  EXPECT_TRUE(hook.Stage() == nullptr);
  EXPECT_TRUE(hook.ActiveEvents().empty());
  EXPECT_TRUE(hook.ActiveTriggerPoints().empty());
  EXPECT_TRUE(hook.NodeEventCallback() == nullptr);
  EXPECT_TRUE(hook.InputEventCallback() == nullptr);
  EXPECT_TRUE(hook.WindowEventCallback() == nullptr);
  EXPECT_TRUE(hook.TriggerCallback() == nullptr);
  EXPECT_TRUE(hook.TriggerRoot() == nullptr);
}

TEST_F(HookTest, ConstructWithTrigger) {
  auto triggerPoint =
      Hook::TriggerPoint{.TriggerType = Hook::TriggerType::AfterUpdateScene};
  auto hook = HookSpy({}, {triggerPoint});

  EXPECT_THAT(hook.ActiveTriggerPoints(), testing::ElementsAre(triggerPoint));
}

TEST_F(HookTest, ConstructWithEvent) {
  auto eventType = Hook::EventType::Input;
  auto hook = HookSpy({eventType});

  EXPECT_THAT(hook.ActiveEvents(), testing::ElementsAre(eventType));
}

TEST_F(HookTest, ActivateTrigger) {
  auto triggerPoint =
      Hook::TriggerPoint{.TriggerType = Hook::TriggerType::AfterUpdateScene};
  auto hook = HookSpy();
  auto stageMock = StageMock();
  hook.SetStage(&stageMock);

  hook.ActivateTrigger(triggerPoint);

  EXPECT_THAT(hook.ActiveTriggerPoints(), testing::ElementsAre(triggerPoint));
  EXPECT_THAT(stageMock.TriggerHooks(),
              testing::ElementsAre(
                  std::pair<soil::stage::hook::TriggerHook::TriggerPoint,
                            std::vector<soil::stage::hook::TriggerHook *>>(
                      triggerPoint, {&hook})));
}

TEST_F(HookTest, ActivateTriggerWithoutStage) {
  auto triggerPoint =
      Hook::TriggerPoint{.TriggerType = Hook::TriggerType::AfterUpdateScene};
  auto hook = HookSpy();

  hook.ActivateTrigger(triggerPoint);

  EXPECT_THAT(hook.ActiveTriggerPoints(), testing::ElementsAre(triggerPoint));
}

TEST_F(HookTest, ActivateEventWithoutStage) {
  auto eventType = Hook::EventType::Input;
  auto hook = HookSpy();

  hook.ActivateEvents({eventType});

  EXPECT_THAT(hook.ActiveEvents(), testing::ElementsAre(eventType));
}

TEST_F(HookTest, ActivateInputEvent) {
  auto eventType = Hook::EventType::Input;
  auto hook = HookSpy();
  auto stageMock = StageMock();
  hook.SetStage(&stageMock);

  hook.ActivateEvents({eventType});

  EXPECT_THAT(hook.ActiveEvents(), testing::ElementsAre(eventType));
  EXPECT_THAT(stageMock.InputEventHooks(), testing::ElementsAre(&hook));
}

TEST_F(HookTest, ActivateStageNodeEvent) {
  auto eventType = Hook::EventType::Node;
  auto hook = HookSpy();
  auto stageMock = StageMock();
  hook.SetStage(&stageMock);

  hook.ActivateEvents({eventType});

  EXPECT_THAT(hook.ActiveEvents(), testing::ElementsAre(eventType));
  EXPECT_THAT(
      stageMock.NodeEventHooks(),
      testing::ElementsAre(
          std::pair<
              scene::Node *,
              std::vector<soil::stage::hook::EventHook<stage::event::Node> *>>(
              nullptr, {&hook})));

  hook.SetStage(nullptr);  // cleanup
}

TEST_F(HookTest, ActivateRootNodeEvent) {
  auto eventType = Hook::EventType::Node;
  auto hook = HookSpy();
  auto stageMock = StageMock();
  hook.SetStage(&stageMock);
  auto anyNode = scene::Node(scene::Node::Type::Transform);

  hook.SetTriggerRoot(&anyNode);
  hook.ActivateEvents({eventType});

  EXPECT_THAT(hook.ActiveEvents(), testing::ElementsAre(eventType));
  EXPECT_THAT(
      stageMock.NodeEventHooks(),
      testing::ElementsAre(
          std::pair<
              scene::Node *,
              std::vector<soil::stage::hook::EventHook<stage::event::Node> *>>(
              &anyNode, {&hook})));

  hook.SetStage(nullptr);  // cleanup
}

TEST_F(HookTest, ActivateWindowEvent) {
  auto eventType = Hook::EventType::Window;
  auto hook = HookSpy();
  auto stageMock = StageMock();
  hook.SetStage(&stageMock);

  hook.ActivateEvents({eventType});

  EXPECT_THAT(hook.ActiveEvents(), testing::ElementsAre(eventType));
  EXPECT_THAT(stageMock.WindowEventHooks(), testing::ElementsAre(&hook));

  hook.SetStage(nullptr);  // cleanup
}

TEST_F(HookTest, UpdateTriggerRoot) {
  auto eventType = Hook::EventType::Node;
  auto hook = HookSpy();
  auto stageMock = StageMock();
  hook.SetStage(&stageMock);
  auto anyNode = scene::Node(scene::Node::Type::Transform);

  hook.ActivateEvents({eventType});
  EXPECT_THAT(
      stageMock.NodeEventHooks(),
      testing::ElementsAre(
          std::pair<
              scene::Node *,
              std::vector<soil::stage::hook::EventHook<stage::event::Node> *>>(
              nullptr, {&hook})));

  hook.SetTriggerRoot(&anyNode);
  EXPECT_THAT(
      stageMock.NodeEventHooks(),
      testing::ElementsAre(
          std::pair<
              scene::Node *,
              std::vector<soil::stage::hook::EventHook<stage::event::Node> *>>(
              &anyNode, {&hook})));

  hook.SetStage(nullptr);  // cleanup
}
}  // namespace soil::stage::scene::component::event
