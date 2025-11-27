#include "stage/scene/component/update_graph_component.h"

#include "../mocks.hpp"
#include "gtest/gtest.h"
#include "stage/mocks.hpp"

namespace soil::stage::scene::component {
class UpdateGraphComponentTest : public testing::Test {};

TEST_F(UpdateGraphComponentTest, Update) {
  UpdateGraphComponent updateGraph;
  auto node = NodeMock();

  ASSERT_EQ(node.Calls.UpdateDirty, 0);
  ASSERT_EQ(updateGraph.DirtyNodes().size(), 0);

  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);

  node.SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node(&node, stage::event::Node::ChangeType::State));
  EXPECT_EQ(updateGraph.DirtyNodes().size(), 1);

  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 1);
  EXPECT_EQ(updateGraph.DirtyNodes().size(), 0);

  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 1);
  EXPECT_EQ(updateGraph.DirtyNodes().size(), 0);
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
  auto node = new NodeMock();

  node->SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node(node, stage::event::Node::ChangeType::State));

  updateGraph.Update();
  EXPECT_EQ(node->Calls.UpdateDirty, 1);

  node->SetDirty(Node::DirtyImpact::Self);
  updateGraph.OnEvent(
      stage::event::Node(node, stage::event::Node::ChangeType::State));
  node->SetState(Node::State::Delete);
  updateGraph.OnEvent(
      stage::event::Node(node, stage::event::Node::ChangeType::State));

  EXPECT_EQ(updateGraph.NodesToDelete().size(), 1);
  updateGraph.Update();
  EXPECT_EQ(updateGraph.NodesToDelete().size(), 0);
  EXPECT_EQ(node->Calls.UpdateDirty, 1);
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
  EXPECT_EQ(updateGraph.NodesToDelete().size(), 0);
  EXPECT_EQ(updateGraph.DirtyNodes().size(), 0);

  updateGraph.Update();
  EXPECT_EQ(node.Calls.UpdateDirty, 0);
  EXPECT_EQ(childNode.Calls.UpdateDirty, 1);
}

TEST_F(UpdateGraphComponentTest, UpdateWithNodesToDelete) {
  StageMock stage;
  SceneMock scene;
  scene.SetStage(&stage);
  UpdateGraphComponent updateGraph;
  stage.AddEventHook(nullptr,
                     (hook::EventHook<stage::event::Node> *)&updateGraph);
  auto node = scene.AddChild(new NodeMock());
  node->StageOverride = &stage;
  auto childNode = node->AddChild(new NodeMock());
  childNode->StageOverride = &stage;

  node->SetState(Node::State::Delete);
  EXPECT_EQ(updateGraph.NodesToDelete().size(), 1);
  EXPECT_EQ(updateGraph.DirtyNodes().size(), 0);

  updateGraph.Update();
  EXPECT_EQ(updateGraph.NodesToDelete().size(), 0);
  EXPECT_EQ(updateGraph.DirtyNodes().size(), 0);
}
}  // namespace soil::stage::scene::component
