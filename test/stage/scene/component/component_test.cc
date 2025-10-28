#include "stage/scene/component/component.h"

#include "../mocks.hpp"
#include "gtest/gtest.h"
#include "mocks.hpp"

namespace soil::stage::scene::component {
class ComponentTest : public testing::Test {};

TEST_F(ComponentTest, Contruct) {
  const auto component = Component(Component::Type::Metadata);

  EXPECT_EQ(component.GetParent(), nullptr);
  EXPECT_EQ(component.GetType(), Component::Type::Metadata);
}

TEST_F(ComponentTest, AddToNode) {
  auto component = Component(Component::Type::Metadata);
  auto node = NodeMock(Node::Type::Game);
  auto node2 = NodeMock(Node::Type::Game);

  node.AddComponent(&component);
  EXPECT_EQ(component.GetParent(), nullptr);

  node.Update();  // apply add
  EXPECT_EQ(component.GetParent(), &node);
  ASSERT_EQ(node.GetFirstComponent(), &component);

  node2.AddComponent(&component);
  node2.Update();  // apply add
  EXPECT_EQ(component.GetParent(), &node2);
  EXPECT_EQ(node.GetFirstComponent(), nullptr);

  node2.RemoveComponent(&component);
}

TEST_F(ComponentTest, SetUpdateType) {
  auto node = NodeMock(Node::Type::Game);
  auto* component =
      node.AddComponent(new ComponentMock(Component::Type::Metadata));
  node.Update();  // apply add

  ASSERT_EQ(node.HandleComponentEventCalledCount, 0);

  component->SetUpdateType(Component::UpdateType::Always);

  EXPECT_EQ(node.HandleComponentEventCalledCount, 1);
}
}  // namespace soil::stage::scene::component
