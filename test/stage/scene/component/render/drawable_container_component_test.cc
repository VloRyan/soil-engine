#include "stage/scene/component/render/drawable_container_component.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "stage/scene/component/render/drawable_component.h"
#include "stage/scene/mocks.hpp"
#include "video/render/draw/mocks.hpp"
#include "video/render/mocks.hpp"

namespace soil::stage::scene::component::render {
class DrawableContainerComponentTest : public testing::Test {
 protected:
  static constexpr video::render::StateIdentifier opaqueStateId = {
      .State = {.Blend = false}};
  static constexpr video::render::StateIdentifier transparentStateId = {
      .State = {.Blend = true}};
};
class DummyDrawableComponent : public DrawableComponent {
 public:
  explicit DummyDrawableComponent(video::render::draw::Drawable& drawable)
      : DrawableObject(drawable) {};
  ~DummyDrawableComponent() override = default;
  [[nodiscard]] video::render::draw::Drawable* Drawable() override {
    return &DrawableObject;
  };
  bool IsDrawablePile() override { return DrawablePile; }
  video::render::draw::Drawable& DrawableObject;
  bool DrawablePile{false};
};

TEST_F(DrawableContainerComponentTest, InsertRemove) {
  auto* container = new video::render::draw::DrawableContainer();
  auto renderStateComp = DrawableContainerComponent(container);

  auto drawable = video::render::draw::DrawableMock(opaqueStateId);
  auto renderableComp = DummyDrawableComponent(drawable);
  auto node = scene::Node(scene::Node::Type::Game);
  auto componentAddedEvent = stage::event::Component(
      &renderableComp, stage::event::Component::TriggerType::Added);
  auto componentRemovedEvent = stage::event::Component(
      &renderableComp, stage::event::Component::TriggerType::Removed);

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentAddedEvent));
  EXPECT_THAT(container->GetDrawables(
                  video::render::draw::DrawableContainer::Kind::Opaque),
              testing::ElementsAre(&drawable));

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentRemovedEvent));
  EXPECT_TRUE(
      container
          ->GetDrawables(video::render::draw::DrawableContainer::Kind::Opaque)
          .empty());
}

TEST_F(DrawableContainerComponentTest, InsertAlreadyInserted) {
  auto* container = new video::render::draw::DrawableContainer();
  auto renderStateComp = DrawableContainerComponent(container);

  auto drawable = video::render::draw::DrawableMock(opaqueStateId);
  auto renderableComp1 = DummyDrawableComponent(drawable);
  auto renderableComp2 = DummyDrawableComponent(drawable);
  auto node = scene::Node(scene::Node::Type::Game);
  auto componentAddedEvent1 = stage::event::Component(
      &renderableComp1, stage::event::Component::TriggerType::Added);
  auto componentAddedEvent2 = stage::event::Component(
      &renderableComp1, stage::event::Component::TriggerType::Added);

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentAddedEvent1));
  EXPECT_THAT(container->GetDrawables(
                  video::render::draw::DrawableContainer::Kind::Opaque),
              testing::ElementsAre(&drawable));

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentAddedEvent2));
  EXPECT_THAT(container->GetDrawables(
                  video::render::draw::DrawableContainer::Kind::Opaque),
              testing::ElementsAre(&drawable));
}

TEST_F(DrawableContainerComponentTest, RemoveDrawablePile) {
  auto* container = new video::render::draw::DrawableContainer();
  auto renderStateComp = DrawableContainerComponent(container);

  auto drawable = video::render::draw::DrawableMock(opaqueStateId);
  auto renderableComp = DummyDrawableComponent(drawable);
  renderableComp.DrawablePile = true;

  auto node = scene::Node(scene::Node::Type::Game);
  auto componentAddedEvent = stage::event::Component(
      &renderableComp, stage::event::Component::TriggerType::Added);
  auto componentRemovedEvent = stage::event::Component(
      &renderableComp, stage::event::Component::TriggerType::Removed);

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentAddedEvent));
  EXPECT_THAT(container->GetDrawables(
                  video::render::draw::DrawableContainer::Kind::Opaque),
              testing::ElementsAre(&drawable));

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentRemovedEvent));
  EXPECT_THAT(container->GetDrawables(
                  video::render::draw::DrawableContainer::Kind::Opaque),
              testing::ElementsAre(&drawable));
}
/*
TEST_F(DrawableContainerComponentTest, Changed) {
  auto* container = new video::render::draw::DrawableContainer();
  auto renderStateComp = DrawableContainerComponent(nullptr, container);

  auto drawable = video::render::draw::DrawableMock(opaqueStateId);
  auto renderableComp = DummyDrawableComponent(drawable);
  auto node = scene::Node(scene::Node::Type::Game);
  auto componentChangedEvent = stage::event::Component(
      &renderableComp, stage::event::Component::TriggerType::Changed);

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentChangedEvent));
  EXPECT_THAT(container->GetDrawables(
                  video::render::draw::DrawableContainer::Kind::Opaque),
              testing::ElementsAre(&drawable));

  drawable.State = transparentStateId;
  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentChangedEvent));
  EXPECT_TRUE(
      container
          ->GetDrawables(video::render::draw::DrawableContainer::Kind::Opaque)
          .empty());
  EXPECT_THAT(container->GetDrawables(
                  video::render::draw::DrawableContainer::Kind::Transparent),
              testing::ElementsAre(&drawable));
}
*/
}  // namespace soil::stage::scene::component::render
