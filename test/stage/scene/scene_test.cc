#include "stage/scene/scene.h"

#include "component/mocks.hpp"
#include "gtest/gtest.h"
#include "hook/mocks.hpp"
#include "mocks.hpp"
#include "stage/stage.h"

namespace soil::stage::scene {

class SceneTest : public testing::Test {};

TEST_F(SceneTest, Contruct) {
  const auto scene = new Scene();

  EXPECT_EQ(scene->GetType(), Node::Type::Scene);
  EXPECT_EQ(scene->GetStage(), nullptr);
  EXPECT_EQ(scene->GetPipeline(), nullptr);
}

TEST_F(SceneTest, Update) {
  auto scene = Scene();
  const auto node = scene.AddChild(new NodeMock(Node::Type::Game));
  auto* child = node->AddChild(new NodeMock(Node::Type::Game));
  auto* childOfChild = child->AddChild(new NodeMock(Node::Type::Game));

  scene.Update();

  EXPECT_EQ(node->UpdateCalledCount, 1);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(childOfChild->UpdateDirtyCalledCount, 1);

  childOfChild->SetDirty(Node::DirtyImpact::Self);
  scene.Update();

  EXPECT_EQ(node->UpdateCalledCount, 1);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(childOfChild->UpdateDirtyCalledCount, 2);

  child->SetDirty(Node::DirtyImpact::Dependents);
  scene.Update();

  EXPECT_EQ(node->UpdateCalledCount, 1);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 2);
  EXPECT_EQ(childOfChild->UpdateDirtyCalledCount, 3);

  node->SetDirty(
      Node::DirtyImpact::Dependents);  // this will also update childOfChild
  childOfChild->SetDirty(Node::DirtyImpact::Self);  // ... will be skipped
  scene.Update();

  EXPECT_EQ(node->UpdateCalledCount, 2);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 2);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 3);
  EXPECT_EQ(childOfChild->UpdateDirtyCalledCount, 4);

  childOfChild->SetDirty(Node::DirtyImpact::Dependents);
  child->SetDirty(
      Node::DirtyImpact::Dependents);  // do not update childOfChild again
  scene.Update();

  EXPECT_EQ(node->UpdateCalledCount, 2);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 2);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 4);
  EXPECT_EQ(childOfChild->UpdateDirtyCalledCount, 5);
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

  EXPECT_EQ(node->UpdateCalledCount, 1);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(childOfChild->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(comp1->UpdateCalledCount, 1);
  EXPECT_EQ(comp2->UpdateCalledCount, 1);
  EXPECT_EQ(comp3->UpdateCalledCount, 1);

  childOfChild->SetDirty(Node::DirtyImpact::Dependents);
  scene->Update();

  EXPECT_EQ(node->UpdateCalledCount, 1);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(childOfChild->UpdateDirtyCalledCount, 2);
  EXPECT_EQ(comp1->UpdateCalledCount, 1);
  EXPECT_EQ(comp2->UpdateCalledCount, 1);
  EXPECT_EQ(comp3->UpdateCalledCount, 2);

  delete scene;
}

TEST_F(SceneTest, Update_Dirty_in_update) {
  auto scene = Scene();
  const auto node = scene.AddChild(new NodeMock(Node::Type::Game));
  const auto child = node->AddChild(new NodeMock(Node::Type::Game));

  scene.Update();

  EXPECT_EQ(node->UpdateCalledCount, 1);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 1);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 1);

  node->SetDirty(Node::DirtyImpact::Self);
  node->UpdateFunc = [child] { child->SetDirty(Node::DirtyImpact::Self); };
  scene.Update();

  EXPECT_EQ(node->UpdateCalledCount, 2);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 2);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 1);

  scene.Update();

  EXPECT_EQ(node->UpdateCalledCount, 2);
  EXPECT_EQ(node->UpdateDirtyCalledCount, 2);
  EXPECT_EQ(child->UpdateDirtyCalledCount, 2);
}

TEST_F(SceneTest, Perform_feature) {
  auto scene = Scene();
  const auto* afterUpdateFeature = scene.AddHook(new hook::UpdateHookMock());
  const auto* renderFeature = scene.AddHook(new hook::RenderHookMock());
  auto state = video::render::State();
  scene.Update();

  EXPECT_EQ(afterUpdateFeature->OnUpdateCalledCount, 1);
  EXPECT_EQ(renderFeature->OnRenderCalledCount, 0);
  scene.Render(state);

  EXPECT_EQ(afterUpdateFeature->OnUpdateCalledCount, 1);
  EXPECT_EQ(renderFeature->OnRenderCalledCount, 1);
}

TEST_F(SceneTest, Remove_hook) {
  auto scene = Scene();
  auto* afterUpdateFeature = scene.AddHook(new hook::UpdateHookMock());
  auto* renderFeature = scene.AddHook(new hook::RenderHookMock());
  auto state = video::render::State();

  scene.Update();
  EXPECT_EQ(afterUpdateFeature->OnUpdateCalledCount, 1);

  scene.Render(state);
  EXPECT_EQ(renderFeature->OnRenderCalledCount, 1);

  scene.RemoveHook(afterUpdateFeature);

  scene.Update();
  EXPECT_EQ(afterUpdateFeature->OnUpdateCalledCount, 1);

  scene.RemoveHook(renderFeature);
  scene.Render(state);
  EXPECT_EQ(renderFeature->OnRenderCalledCount, 1);
}

TEST_F(SceneTest, Update_active_node) {
  auto scene = Scene();
  const auto activeNode = scene.AddChild(new NodeMock(Node::Type::Game));
  activeNode->SetUpdateType(Node::UpdateType::Active);
  const auto normalNode = scene.AddChild(new NodeMock(Node::Type::Game));

  scene.Update();

  EXPECT_EQ(activeNode->UpdateCalledCount, 1);
  EXPECT_EQ(normalNode->UpdateCalledCount, 1);

  scene.Update();

  EXPECT_EQ(activeNode->UpdateCalledCount, 2);
  EXPECT_EQ(normalNode->UpdateCalledCount, 1);
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
      input::Keys::A, input::Event::State::Press);
  const auto windowEvent = WindowEvent(nullptr, WindowEvent::SizeChanged);

  scene.Handle(inputEvent);
  scene.Handle(windowEvent);
  scene.Update();
  ASSERT_EQ(node.HandleInputEventCalledCount, 1);
  ASSERT_EQ(node.HandleWindowEventCalledCount, 1);
  ASSERT_EQ(child.HandleInputEventCalledCount, 1);
  ASSERT_EQ(child.HandleWindowEventCalledCount, 1);
  ASSERT_EQ(activeNode.UpdateCalledCount, 1);

  node.RemoveChild(&child);

  scene.Handle(inputEvent);
  scene.Handle(windowEvent);
  scene.Update();
  EXPECT_EQ(node.HandleInputEventCalledCount, 2);
  EXPECT_EQ(node.HandleWindowEventCalledCount, 2);
  EXPECT_EQ(child.HandleInputEventCalledCount, 1);
  EXPECT_EQ(child.HandleWindowEventCalledCount, 1);
  EXPECT_EQ(activeNode.UpdateCalledCount, 2);

  scene.RemoveChild(&node);
  scene.RemoveChild(&activeNode);

  scene.Handle(inputEvent);
  scene.Handle(windowEvent);
  EXPECT_EQ(node.HandleInputEventCalledCount, 2);
  EXPECT_EQ(node.HandleWindowEventCalledCount, 2);
  EXPECT_EQ(child.HandleInputEventCalledCount, 1);
  EXPECT_EQ(child.HandleWindowEventCalledCount, 1);
  EXPECT_EQ(activeNode.UpdateCalledCount, 2);
}

TEST_F(SceneTest, Remove_active_node_on_node_remove) {
  auto scene = Scene();
  auto* node = scene.AddChild(new NodeMock(Node::Type::Game));
  const auto activeNode = node->AddChild(new NodeMock(Node::Type::Game));
  activeNode->SetUpdateType(Node::UpdateType::Active);
  const auto normalNode = node->AddChild(new NodeMock(Node::Type::Game));

  scene.Update();

  EXPECT_EQ(activeNode->UpdateCalledCount, 1);
  EXPECT_EQ(normalNode->UpdateDirtyCalledCount, 1);

  scene.Update();

  EXPECT_EQ(activeNode->UpdateCalledCount, 2);
  EXPECT_EQ(normalNode->UpdateCalledCount, 0);

  delete node;

  scene.Update();
}

TEST_F(SceneTest, Change_updateType) {
  auto scene = Scene();
  auto node = NodeMock(Node::Type::Game);
  node.SetUpdateType(Node::UpdateType::Passive);
  scene.AddChild(&node);

  scene.Update();
  EXPECT_EQ(node.UpdateCalledCount, 1);

  scene.Update();
  EXPECT_EQ(node.UpdateCalledCount, 1);

  node.SetUpdateType(Node::UpdateType::Active);

  scene.Update();
  EXPECT_EQ(node.UpdateCalledCount, 2);

  node.SetUpdateType(Node::UpdateType::Passive);

  scene.Update();
  EXPECT_EQ(node.UpdateCalledCount, 2);
}
}  // namespace soil::stage::scene
