#include "stage/scene/render/plain.h"

#include <gtest/gtest.h>

#include "mocks.hpp"
#include "video/render/renderable_group.h"
namespace soil::stage::scene::render {
class PlainTest : public testing::Test {};
/*
TEST_F(PlainTest, OnAdded) {
  auto renderContainer = RenderContainerMock();
  auto plain = Plain(&renderContainer);
  auto component = VisualComponentMock(render::Type::Plain, true, true);
  plain.Handle(
      event::Component(&component, event::Component::TriggerType::Added));

  EXPECT_EQ(renderContainer.Calls.Add, 0);  // still in added state

  plain.OnTrigger(hook::TriggerHook::TriggerType::AfterUpdateScene);

  EXPECT_EQ(renderContainer.Calls.Add, 1);
}
TEST_F(PlainTest, OnChanged) {
  auto renderContainer = RenderContainerMock();
  auto plain = Plain(&renderContainer);
  auto component = VisualComponentMock(render::Type::Plain, true, true);

  plain.Handle(
      event::Component(&component, event::Component::TriggerType::Added));
  plain.OnTrigger(hook::TriggerHook::TriggerType::AfterUpdateScene);
  EXPECT_EQ(renderContainer.Calls.Add, 1);

  plain.Handle(event::Component(&component,
                                event::Component::TriggerType::Changed,
                                event::Component::ChangeType::Data));
  EXPECT_EQ(renderContainer.Calls.Add, 1);
}

TEST_F(PlainTest, OnChangedWithoutAdded) {
  auto renderContainer = RenderContainerMock();
  auto plain = Plain(&renderContainer);
  auto component = VisualComponentMock(render::Type::Plain, true, true);
  plain.Handle(event::Component(&component,
                                event::Component::TriggerType::Changed,
                                event::Component::ChangeType::Data));

  EXPECT_EQ(renderContainer.Calls.Add, 0);  //  no effect

  plain.OnTrigger(hook::TriggerHook::TriggerType::AfterUpdateScene);

  EXPECT_EQ(renderContainer.Calls.Add, 1);
}

TEST_F(PlainTest, OnChangedBeforeFirstRender) {
  auto renderContainer = RenderContainerMock();
  auto plain = Plain(&renderContainer);
  auto component = VisualComponentMock(render::Type::Plain, true, true);
  plain.Handle(
      event::Component(&component, event::Component::TriggerType::Added));

  plain.Handle(event::Component(&component,
                                event::Component::TriggerType::Changed,
                                event::Component::ChangeType::Data));

  EXPECT_EQ(renderContainer.Calls.Add, 0);
  EXPECT_EQ(renderContainer.Calls.Remove, 0);

  component.SetVisible(false);
  component.Update();  // reset dirty state
  plain.Handle(event::Component(&component,
                                event::Component::TriggerType::Changed,
                                event::Component::ChangeType::Data));
  EXPECT_EQ(renderContainer.Calls.Add, 0);
  EXPECT_EQ(renderContainer.Calls.Remove, 0);

  plain.OnTrigger(hook::TriggerHook::TriggerType::AfterUpdateScene);
  EXPECT_EQ(renderContainer.Calls.Add, 0);
  EXPECT_EQ(renderContainer.Calls.Remove, 0);
}
*/
}  // namespace soil::stage::scene::render
