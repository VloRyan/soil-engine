#include "stage/scene/component/update_graph_component.h"

#include "../mocks.hpp"
#include "gtest/gtest.h"

namespace soil::stage::scene::component {
class UpdateGraphComponentTest : public testing::Test {};

TEST_F(UpdateGraphComponentTest, Update) {
  UpdateGraphComponent updateGraph;
  auto node = NodeMock();

  ASSERT_EQ(node.Calls.UpdateDirty, 0);

  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);

  node.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node(&node, stage::event::Node::ChangeType::State));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 1);

  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 1);
}

TEST_F(UpdateGraphComponentTest, UpdateAfterAdd) {
  UpdateGraphComponent updateGraph;
  auto node = NodeMock();
  auto &childNode = *node.AddChild(new NodeMock());

  updateGraph.OnEvent(
      stage::event::Node::MakeChildAddedEvent(&node, &childNode));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);
  EXPECT_EQ(childNode.Calls.UpdateDirty, 0);

  node.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node::MakeChildAddedEvent(&node, &childNode));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);
  EXPECT_EQ(childNode.Calls.UpdateDirty, 0);

  childNode.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node::MakeChildAddedEvent(&node, &childNode));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);
  EXPECT_EQ(childNode.Calls.UpdateDirty, 1);
}

TEST_F(UpdateGraphComponentTest, UpdateTopDirty) {
  UpdateGraphComponent updateGraph;
  auto node = NodeMock();
  auto &childNode = *node.AddChild(new NodeMock());

  node.SetDirty(Node::DirtyImpact::Self);
  childNode.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node(&childNode, stage::event::Node::ChangeType::State));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);
  EXPECT_EQ(childNode.Calls.UpdateDirty, 1);

  node.SetDirty(Node::DirtyImpact::Dependents);
  childNode.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node(&childNode, stage::event::Node::ChangeType::State));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 1);  // top dirty
  EXPECT_EQ(childNode.Calls.UpdateDirty, 2);
}

TEST_F(UpdateGraphComponentTest, UpdateAfterDelete) {
  UpdateGraphComponent updateGraph;
  auto node = NodeMock();

  node.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node(&node, stage::event::Node::ChangeType::State));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 1);

  node.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node(&node, stage::event::Node::ChangeType::State));
  node.SetState(Node::State::Delete);
  updateGraph.OnEvent(
      stage::event::Node(&node, stage::event::Node::ChangeType::State));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 1);
}

TEST_F(UpdateGraphComponentTest, UpdateAfterChildRemoved) {
  UpdateGraphComponent updateGraph;
  auto node = NodeMock();
  auto &childNode = *node.AddChild(new NodeMock());

  updateGraph.OnEvent(
      stage::event::Node::MakeChildAddedEvent(&node, &childNode));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);
  EXPECT_EQ(childNode.Calls.UpdateDirty, 0);

  childNode.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node::MakeChildAddedEvent(&node, &childNode));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);
  EXPECT_EQ(childNode.Calls.UpdateDirty, 1);

  childNode.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node::MakeChildAddedEvent(&node, &childNode));
  updateGraph.OnEvent(
      stage::event::Node::MakeChildRemovedEvent(&node, &childNode));
  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);
  EXPECT_EQ(childNode.Calls.UpdateDirty, 1);
}

/*
TEST_F(UpdateGraphComponent, UpdateDirtySelf) {
  auto node = Node(Node::Type::Transform);
  const auto child = new NodeMock();
  auto* comp = node.AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  node.Update();       // update to set comp
  comp->ResetMocks();  // UpdateMatrix is called on add
  child->ResetMocks();

  node.SetDirty(Node::DirtyImpact::Self);
  node.Update();

  EXPECT_FALSE(node.IsDirty());
  EXPECT_FALSE(node.IsDirtyImpact(Node::DirtyImpact::Self));
  EXPECT_EQ(comp->UpdateCalledCount, 0);

  EXPECT_EQ(child->UpdateCalledCount, 0);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 0);
}
/*
TEST_F(NodeTest, UpdateDirtyComponent) {
  auto node = Node(Node::Type::Transform);
  const auto child = node.AddChild(new NodeMock());
  auto* comp = node.AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  node.Update();       // update to set comp
  comp->ResetMocks();  // UpdateMatrix is called on add
  child->ResetMocks();

  node.SetDirty(Node::DirtyImpact::Components);
  node.Update();

  EXPECT_FALSE(node.IsDirty());
  EXPECT_FALSE(node.IsDirtyImpact(Node::DirtyImpact::Components));
  EXPECT_EQ(comp->UpdateCalledCount, 1);

  EXPECT_EQ(child->UpdateCalledCount, 0);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 0);
}
*//*
std::bitset<4> toBitset(std::vector<Node::DirtyImpact> impacts) {
  auto bits = std::bitset<4>();
  for (auto impact : impacts) {
    bits[static_cast<std::int8_t>(impact)] = true;
  }
  return bits;
}
TEST_F(UpdateGraphComponent, UpdateDirtyDependents) {
  auto node = Node(Node::Type::Transform);
  auto* child = node.AddChild(new NodeMock());
  auto* childOfChild = child->AddChild(new NodeMock());
  auto* comp = node.AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  auto* childComp = child->AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  auto* childOfChildComp = child->AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  node.Update();       // update to set comp
  comp->ResetMocks();  // UpdateMatrix is called on add
  childComp->ResetMocks();
  childOfChildComp->ResetMocks();
  child->ResetMocks();
  childOfChild->ResetMocks();

  node.SetDirty(Node::DirtyImpact::Dependents);
  node.Update();

  EXPECT_FALSE(node.IsDirty());
  EXPECT_FALSE(node.IsDirtyImpact(Node::DirtyImpact::Dependents));
  EXPECT_EQ(comp->UpdateCalledCount, 1);

  EXPECT_EQ(child->UpdateCalledCount, 0);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 1);
  EXPECT_THAT(
      child->UpdateDirtyImpacts,
      toBitset({Node::DirtyImpact::Components, Node::DirtyImpact::Dependents}));
  EXPECT_EQ(childComp->UpdateCalledCount, 1);

  EXPECT_EQ(childOfChild->UpdateCalledCount, 0);
  EXPECT_EQ(childOfChild->UpdateDirtyCalledCount, 1);
  EXPECT_THAT(childOfChild->UpdateDirtyImpacts,
              toBitset({Node::DirtyImpact::Dependents}));
  EXPECT_EQ(childOfChildComp->UpdateCalledCount, 1);
}

TEST_F(UpdateGraphComponent, UpdateDirtyTransform) {
  auto node = Node(Node::Type::Transform);
  auto* child = node.AddChild(new NodeMock());
  auto* childOfChild = child->AddChild(new NodeMock());
  auto* comp = node.AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  auto* childComp = child->AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  auto* childOfChildComp = child->AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  node.Update();       // update to set comp
  comp->ResetMocks();  // UpdateMatrix is called on add
  childComp->ResetMocks();
  childOfChildComp->ResetMocks();
  child->ResetMocks();
  childOfChild->ResetMocks();

  node.SetDirty(Node::DirtyImpact::Transform);
  node.Update();

  EXPECT_FALSE(node.IsDirty());
  EXPECT_FALSE(node.IsDirtyImpact(Node::DirtyImpact::Transform));
  EXPECT_EQ(comp->UpdateCalledCount, 1);

  EXPECT_EQ(child->UpdateCalledCount, 0);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 1);
  EXPECT_THAT(
      child->UpdateDirtyImpacts,
      toBitset({Node::DirtyImpact::Transform, Node::DirtyImpact::Components}));
  EXPECT_EQ(childComp->UpdateCalledCount, 1);

  EXPECT_EQ(childOfChild->UpdateCalledCount, 0);
  EXPECT_EQ(childOfChild->UpdateDirtyCalledCount, 1);
  EXPECT_THAT(childOfChild->UpdateDirtyImpacts,
              toBitset({Node::DirtyImpact::Transform}));
  EXPECT_EQ(childOfChildComp->UpdateCalledCount, 1);
}

TEST_F(UpdateGraphComponent, UpdateAlwaysUpdateComponent) {
  auto node = NodeMock();
  auto* comp = node.AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  auto* alwaysUpdateComp = node.AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  alwaysUpdateComp->SetUpdateType(component::Component::UpdateType::Always);

  ASSERT_EQ(comp->UpdateCalledCount, 0);
  ASSERT_EQ(alwaysUpdateComp->UpdateCalledCount, 0);

  node.Update();
  EXPECT_EQ(comp->UpdateCalledCount, 1);
  EXPECT_EQ(alwaysUpdateComp->UpdateCalledCount, 1);

  node.Update();
  EXPECT_EQ(comp->UpdateCalledCount, 1);
  EXPECT_EQ(alwaysUpdateComp->UpdateCalledCount, 2);

  alwaysUpdateComp->SetUpdateType(
      component::Component::UpdateType::WhenNodeDirty);
  node.Update();
  EXPECT_EQ(comp->UpdateCalledCount, 1);
  EXPECT_EQ(alwaysUpdateComp->UpdateCalledCount, 2);
}

TEST_F(UpdateGraphComponent, HandleTransformChanges) {
  auto node = NodeMock();
  ASSERT_EQ(node.GetState(), Node::State::Normal);

  node.Transform().SetPosition(glm::vec3(1.F));
  EXPECT_EQ(node.GetState(), Node::State::Dirty);
  EXPECT_TRUE(node.IsDirtyImpact(Node::DirtyImpact::Transform));
}*/
}  // namespace soil::stage::scene::component
