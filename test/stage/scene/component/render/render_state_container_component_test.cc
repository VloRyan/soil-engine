#include "stage/scene/component/render/render_state_container_component.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "stage/scene/component/render/renderable_component.h"
#include "stage/scene/mocks.hpp"
#include "video/render/mocks.hpp"

namespace soil::stage::scene::component::render {
class RenderStateContainerComponentTest : public testing::Test {};
class DummyRenderableComponent : public RenderableComponent {
 public:
  explicit DummyRenderableComponent(video::render::Renderable& renderable)
      : Renderable(renderable) {};
  ~DummyRenderableComponent() override = default;
  [[nodiscard]] video::render::Renderable* GetRenderable() override {
    return &Renderable;
  };
  video::render::Renderable& Renderable;
};

TEST_F(RenderStateContainerComponentTest, Insert) {
  auto* container = new video::render::StateContainerMock();
  auto renderStateComp = RenderStateContainerComponent(container);

  auto renderable = video::render::RenderableMock({});
  auto renderableComp = DummyRenderableComponent(renderable);
  auto node = scene::Node(scene::Node::Type::Game);
  auto componentAddedEvent = stage::event::Component(
      &renderableComp, stage::event::Component::TriggerType::Added);

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentAddedEvent));
  EXPECT_THAT(container->Inserted, testing::ElementsAre(&renderable));
  EXPECT_TRUE(container->Removed.empty());
}

TEST_F(RenderStateContainerComponentTest, Remove) {
  auto* container = new video::render::StateContainerMock();
  auto renderStateComp = RenderStateContainerComponent(container);

  auto renderable = video::render::RenderableMock({});
  auto node = scene::Node(scene::Node::Type::Game);
  auto renderableComp =
      node.AddComponent(new DummyRenderableComponent(renderable));
  auto componentRemovedEvent = stage::event::Component(
      renderableComp, stage::event::Component::TriggerType::Removed);

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentRemovedEvent));
  EXPECT_THAT(container->Removed, testing::ElementsAre(&renderable));
  EXPECT_TRUE(container->Inserted.empty());
  container->ResetMocks();

  auto child = node.AddChild(new scene::Node(scene::Node::Type::Game));
  auto childRenderable = video::render::RenderableMock({});
  child->AddComponent(new DummyRenderableComponent(childRenderable));
  child->Update();

  renderStateComp.OnEvent(
      stage::event::Node::MakeChildRemovedEvent(&node, child));
  EXPECT_THAT(container->Removed, testing::ElementsAre(&childRenderable));
  EXPECT_TRUE(container->Inserted.empty());
  container->ResetMocks();

  node.Update();
  renderStateComp.OnEvent(stage::event::Node::MakeNodeDeletedEvent(&node));
  EXPECT_THAT(container->Removed,
              testing::ElementsAre(&renderable, &childRenderable));
  EXPECT_TRUE(container->Inserted.empty());
}

TEST_F(RenderStateContainerComponentTest, Changed) {
  auto* container = new video::render::StateContainerMock();
  auto renderStateComp = RenderStateContainerComponent(container);

  auto renderable = video::render::RenderableMock({});
  auto renderableComp = DummyRenderableComponent(renderable);
  auto node = scene::Node(scene::Node::Type::Game);
  auto componentAddedEvent = stage::event::Component(
      &renderableComp, stage::event::Component::TriggerType::Changed);

  renderStateComp.OnEvent(
      stage::event::Node::MakeComponentEvent(&node, componentAddedEvent));
  EXPECT_THAT(container->Inserted, testing::ElementsAre(&renderable));
  EXPECT_THAT(container->Removed, testing::ElementsAre(&renderable));
}

}  // namespace soil::stage::scene::component::render
