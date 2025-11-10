#include "stage/scene/scene.h"

#include "gtest/gtest.h"
#include "stage/stage.h"

namespace soil::stage::scene {

class SceneTest : public testing::Test {};

TEST_F(SceneTest, Contruct) {
  const auto scene = new Scene();

  EXPECT_EQ(scene->GetType(), Node::Type::Scene);
  EXPECT_EQ(scene->GetStage(), nullptr);
}
/*
TEST_F(SceneTest, Update) {
  auto scene = Scene();
  const auto node = scene.AddChild(new NodeMock(Node::Type::Game));
  auto* child = node->AddChild(new NodeMock(Node::Type::Game));
  auto* childOfChild = child->AddChild(new NodeMock(Node::Type::Game));

  scene.Update();

  EXPECT_EQ(node->Calls.Update, 0);
  EXPECT_EQ(node->Calls.UpdateDirty, 0);
  EXPECT_EQ(child->Calls.Update, 0);
  EXPECT_EQ(child->Calls.UpdateDirty, 0);
  EXPECT_EQ(childOfChild->Calls.Update, 0);
  EXPECT_EQ(childOfChild->Calls.UpdateDirty, 0);

  childOfChild->SetDirty(Node::DirtyImpact::Self);
  scene.Update();

  EXPECT_EQ(node->Calls.Update, 0);
  EXPECT_EQ(node->Calls.UpdateDirty, 0);
  EXPECT_EQ(child->Calls.Update, 0);
  EXPECT_EQ(child->Calls.UpdateDirty, 0);
  EXPECT_EQ(childOfChild->Calls.Update, 1);
  EXPECT_EQ(childOfChild->Calls.UpdateDirty, 1);

  child->SetDirty(Node::DirtyImpact::Dependents);
  scene.Update();

  EXPECT_EQ(node->Calls.Update, 0);
  EXPECT_EQ(node->Calls.UpdateDirty, 0);
  EXPECT_EQ(child->Calls.Update, 1);
  EXPECT_EQ(child->Calls.UpdateDirty, 1);
  EXPECT_EQ(childOfChild->Calls.Update, 2);
  EXPECT_EQ(childOfChild->Calls.UpdateDirty, 2);

  node->SetDirty(
      Node::DirtyImpact::Dependents);  // this will also update childOfChild
  childOfChild->SetDirty(Node::DirtyImpact::Self);  // ... will be ignored
  scene.Update();

  EXPECT_EQ(node->Calls.Update, 1);
  EXPECT_EQ(node->Calls.UpdateDirty, 1);
  EXPECT_EQ(child->Calls.Update, 2);
  EXPECT_EQ(child->Calls.UpdateDirty, 2);
  EXPECT_EQ(childOfChild->Calls.Update, 3);
  EXPECT_EQ(childOfChild->Calls.UpdateDirty, 3);

  childOfChild->SetDirty(Node::DirtyImpact::Dependents);
  child->SetDirty(
      Node::DirtyImpact::Dependents);  // do not update childOfChild again
  scene.Update();

  EXPECT_EQ(node->Calls.Update, 1);
  EXPECT_EQ(node->Calls.UpdateDirty, 1);
  EXPECT_EQ(child->Calls.Update, 3);
  EXPECT_EQ(child->Calls.UpdateDirty, 3);
  EXPECT_EQ(childOfChild->Calls.Update, 4);
  EXPECT_EQ(childOfChild->Calls.UpdateDirty, 4);
}

TEST_F(SceneTest, AddChild_with_tree_and_destructor) {
  auto* scene = new Scene();
  const auto node = new NodeMock(Node::Type::Game);
  const auto* comp1 = node->AddComponent(
      new component::ComponentMock(component::Component::Type::Visual));
  const auto child = node->AddChild(new NodeMock(Node::Type::Game));
  const auto* comp2 = child->AddComponent(
      new component::ComponentMock(component::Component::Type::Visual));
  const auto childOfChild = child->AddChild(new NodeMock(Node::Type::Game));
  const auto* comp3 = childOfChild->AddComponent(
      new component::ComponentMock(component::Component::Type::Visual));

  scene->AddChild(node);
  scene->Update();

  EXPECT_EQ(node->Calls.Update, 0);
  EXPECT_EQ(node->Calls.UpdateDirty, 0);
  EXPECT_EQ(child->Calls.UpdateDirty, 0);
  EXPECT_EQ(childOfChild->Calls.UpdateDirty, 0);
  EXPECT_EQ(comp1->UpdateCalledCount, 0);
  EXPECT_EQ(comp2->UpdateCalledCount, 0);
  EXPECT_EQ(comp3->UpdateCalledCount, 0);

  childOfChild->SetDirty(Node::DirtyImpact::Dependents);
  scene->Update();

  EXPECT_EQ(node->Calls.Update, 0);
  EXPECT_EQ(node->Calls.UpdateDirty, 0);
  EXPECT_EQ(child->Calls.UpdateDirty, 0);
  EXPECT_EQ(childOfChild->Calls.UpdateDirty, 1);
  EXPECT_EQ(comp1->UpdateCalledCount, 0);
  EXPECT_EQ(comp2->UpdateCalledCount, 0);
  EXPECT_EQ(comp3->UpdateCalledCount, 0);

  delete scene;
}

TEST_F(SceneTest, Update_Dirty_in_update) {
  auto scene = Scene();
  const auto node = scene.AddChild(new NodeMock(Node::Type::Game));
  const auto child = node->AddChild(new NodeMock(Node::Type::Game));

  scene.Update();

  EXPECT_EQ(node->Calls.Update, 0);
  EXPECT_EQ(node->Calls.UpdateDirty, 1);
  EXPECT_EQ(child->Calls.UpdateDirty, 1);

  node->SetDirty(Node::DirtyImpact::Self);
  node->UpdateFunc = [child] { child->SetDirty(Node::DirtyImpact::Self); };
  scene.Update();

  EXPECT_EQ(node->Calls.Update, 1);
  EXPECT_EQ(node->Calls.UpdateDirty, 2);
  EXPECT_EQ(child->Calls.UpdateDirty, 1);

  scene.Update();

  EXPECT_EQ(node->Calls.Update, 1);
  EXPECT_EQ(node->Calls.UpdateDirty, 2);
  EXPECT_EQ(child->Calls.UpdateDirty, 2);
}
/*
TEST_F(SceneTest, Perform_feature) {
  auto scene = Scene();
  auto* afterUpdateFeature = scene.AddHook(
      new hook::HookMock({hook::Hook::Trigger_t::AfterUpdateScene}));
  auto* renderFeature =
      scene.AddHook(new hook::HookMock({hook::Hook::Trigger_t::Render}));
  auto state = video::render::State();
  scene.Update();

  EXPECT_EQ(afterUpdateFeature->Calls.Perform, 1);
  EXPECT_EQ(renderFeature->Calls.Perform, 0);
  scene.Render(state);

  EXPECT_EQ(afterUpdateFeature->Calls.Perform, 1);
  EXPECT_EQ(renderFeature->Calls.Perform, 1);
}

TEST_F(SceneTest, Remove_hook) {
  auto scene = Scene();
  auto* afterUpdateFeature = scene.AddHook(
      new hook::HookMock({hook::Hook::Trigger_t::AfterUpdateScene}));
  auto* renderFeature =
      scene.AddHook(new hook::HookMock({hook::Hook::Trigger_t::Render}));
  auto state = video::render::State();

  scene.Update();
  EXPECT_EQ(afterUpdateFeature->Calls.Perform, 1);
  EXPECT_EQ(renderFeature->Calls.Perform, 0);

  scene.Render(state);
  EXPECT_EQ(afterUpdateFeature->Calls.Perform, 1);
  EXPECT_EQ(renderFeature->Calls.Perform, 1);

  scene.RemoveHook(afterUpdateFeature);

  scene.Update();
  EXPECT_EQ(afterUpdateFeature->Calls.Perform, 1);
  EXPECT_EQ(renderFeature->Calls.Perform, 1);

  scene.RemoveHook(renderFeature);
  scene.Render(state);
  EXPECT_EQ(afterUpdateFeature->Calls.Perform, 1);
  EXPECT_EQ(renderFeature->Calls.Perform, 1);
}

TEST_F(SceneTest, Update_active_node) {
  auto scene = Scene();
  const auto activeNode = scene.AddChild(new NodeMock(Node::Type::Game));
  activeNode->SetUpdateType(Node::UpdateType::Active);
  const auto normalNode = scene.AddChild(new NodeMock(Node::Type::Game));

  scene.Update();

  EXPECT_EQ(activeNode->Calls.Update, 1);
  EXPECT_EQ(normalNode->Calls.Update, 0);

  scene.Update();

  EXPECT_EQ(activeNode->Calls.Update, 2);
  EXPECT_EQ(normalNode->Calls.Update, 0);
}

TEST_F(SceneTest, Remove_listener_on_node_remove) {
  auto scene = Scene();
  auto node = NodeMock(Node::Type::Game);
  node.SetReceiverType(Node::ReceiverType::Input);
  node.SetReceiverType(Node::ReceiverType::Window);
  scene.AddChild(&node);

  auto child = NodeMock(Node::Type::Game);
  child.SetReceiverType(Node::ReceiverType::Input);
  child.SetReceiverType(Node::ReceiverType::Window);
  node.AddChild(&child);
  auto activeNode = NodeMock(Node::Type::Game);
  activeNode.SetUpdateType(Node::UpdateType::Active);
  scene.AddChild(&activeNode);

  const auto inputEvent = input::Event::MakeKeyChangedEvent(
      input::Keys::A, input::Event::StateType::Press);
  const auto windowEvent = WindowEvent(nullptr, WindowEvent::SizeChanged);

  scene.Handle(inputEvent);
  scene.Handle(windowEvent);
  scene.Update();
  ASSERT_EQ(node.Calls.HandleInputEvent, 1);
  ASSERT_EQ(node.Calls.HandleWindowEvent, 1);
  ASSERT_EQ(child.Calls.HandleInputEvent, 1);
  ASSERT_EQ(child.Calls.HandleWindowEvent, 1);
  ASSERT_EQ(activeNode.Calls.Update, 1);

  node.RemoveChild(&child);

  scene.Handle(inputEvent);
  scene.Handle(windowEvent);
  scene.Update();
  EXPECT_EQ(node.Calls.HandleInputEvent, 2);
  EXPECT_EQ(node.Calls.HandleWindowEvent, 2);
  EXPECT_EQ(child.Calls.HandleInputEvent, 1);
  EXPECT_EQ(child.Calls.HandleWindowEvent, 1);
  EXPECT_EQ(activeNode.Calls.Update, 2);

  scene.RemoveChild(&node);
  scene.RemoveChild(&activeNode);

  scene.Handle(inputEvent);
  scene.Handle(windowEvent);
  EXPECT_EQ(node.Calls.HandleInputEvent, 2);
  EXPECT_EQ(node.Calls.HandleWindowEvent, 2);
  EXPECT_EQ(child.Calls.HandleInputEvent, 1);
  EXPECT_EQ(child.Calls.HandleWindowEvent, 1);
  EXPECT_EQ(activeNode.Calls.Update, 2);
}

TEST_F(SceneTest, Remove_active_node_on_node_remove) {
  auto scene = Scene();
  auto* node = scene.AddChild(new NodeMock(Node::Type::Game));
  const auto activeNode = node->AddChild(new NodeMock(Node::Type::Game));
  activeNode->SetUpdateType(Node::UpdateType::Active);
  const auto normalNode = node->AddChild(new NodeMock(Node::Type::Game));

  scene.Update();

  EXPECT_EQ(activeNode->Calls.Update, 1);
  EXPECT_EQ(normalNode->Calls.UpdateDirty, 1);

  scene.Update();

  EXPECT_EQ(activeNode->Calls.Update, 2);
  EXPECT_EQ(normalNode->Calls.Update, 0);

  delete node;

  scene.Update();
}

TEST_F(SceneTest, Change_updateType) {
  auto scene = Scene();
  auto node = NodeMock(Node::Type::Game);
  node.SetUpdateType(Node::UpdateType::Passive);
  scene.AddChild(&node);

  scene.Update();
  EXPECT_EQ(node.Calls.Update, 0);

  scene.Update();
  EXPECT_EQ(node.Calls.Update, 0);

  node.SetUpdateType(Node::UpdateType::Active);

  scene.Update();
  EXPECT_EQ(node.Calls.Update, 1);

  node.SetUpdateType(Node::UpdateType::Passive);

  scene.Update();
  EXPECT_EQ(node.Calls.Update, 1);
}
 */
}  // namespace soil::stage::scene
