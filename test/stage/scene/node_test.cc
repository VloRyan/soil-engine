#include "stage/scene/node.h"

#include <glm/ext/matrix_transform.hpp>
#include <iomanip>
#include <utility>

#include "component/mocks.hpp"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "mocks.hpp"
#include "stage/scene/component/transform_component.h"
#include "stage/stage.h"
namespace soil::stage::scene {
class NodeTest : public testing::Test {};

TEST_F(NodeTest, Contruct) {
  const auto node = Node(Node::Type::Transform);

  EXPECT_EQ(node.GetParent(), nullptr);
  EXPECT_EQ(node.GetType(), Node::Type::Transform);
  EXPECT_EQ(node.GetState(), Node::State::Normal);
  EXPECT_TRUE(node.IsReceiverOf(Node::ReceiverType::None));
  EXPECT_FALSE(node.IsReceiverOf(Node::ReceiverType::Input));
  EXPECT_FALSE(node.IsReceiverOf(Node::ReceiverType::Window));

  std::vector<component::Component*> comps;
  node.GetComponents(comps);
  EXPECT_THAT(comps, testing::ElementsAre(&node.Transform()));
}

TEST_F(NodeTest, AddChild) {
  auto parent = NodeMock();
  auto child = Node(Node::Type::Transform);
  ASSERT_EQ(parent.GetState(), Node::State::Normal);

  parent.AddChild(&child);

  EXPECT_EQ(parent.GetState(), Node::State::Normal);
  EXPECT_EQ(child.GetState(), Node::State::Normal);
  EXPECT_EQ(child.GetParent(), &parent);
  EXPECT_THAT(parent.GetChildren(), testing::ElementsAre(&child));
  EXPECT_THAT(parent.AddedChildren, testing::ElementsAre(&child));
}

TEST_F(NodeTest, ChildDestructor) {
  auto parent = NodeMock();
  auto* child = new Node(Node::Type::Transform);
  parent.AddChild(child);
  ASSERT_FALSE(parent.GetChildren().empty());

  delete child;

  ASSERT_THAT(parent.RemovedChildren, testing::ElementsAre(child));
  EXPECT_TRUE(parent.GetChildren().empty());
}

TEST_F(NodeTest, AddComponent) {
  auto node = Node(Node::Type::Transform);
  auto transformComp =
      dynamic_cast<const component::Component*>(&node.Transform());
  auto* comp =
      new component::ComponentMock(component::Component::Type::Metadata);
  std::vector<component::Component*> components;

  node.GetComponents(components);
  EXPECT_THAT(components, testing::ElementsAre(transformComp));

  node.AddComponent(comp);

  components.clear();
  node.GetComponents(components);
  EXPECT_THAT(components, testing::ElementsAre(transformComp));

  node.Update();

  components.clear();
  node.GetComponents(components);
  EXPECT_THAT(components, testing::ElementsAre(transformComp, comp));
}

TEST_F(NodeTest, RemoveComponent) {
  auto node = Node(Node::Type::Transform);
  auto transformComp =
      dynamic_cast<const component::Component*>(&node.Transform());
  auto* comp =
      new component::ComponentMock(component::Component::Type::Metadata);
  auto* comp2 =
      new component::ComponentMock(component::Component::Type::Metadata);
  std::vector<component::Component*> components;
  node.AddComponent(comp);
  node.AddComponent(comp2);
  node.Update();
  node.GetComponents(components);
  ASSERT_THAT(components, testing::ElementsAre(transformComp, comp, comp2));

  node.RemoveComponent(comp);

  components.clear();
  node.GetComponents(components);
  EXPECT_THAT(components, testing::ElementsAre(transformComp, comp2));

  delete comp2;
  components.clear();
  node.GetComponents(components);
  EXPECT_THAT(components, testing::ElementsAre(transformComp));
}

TEST_F(NodeTest, RemoveComponentJustAdded) {
  auto node = Node(Node::Type::Transform);
  auto transformComp =
      dynamic_cast<const component::Component*>(&node.Transform());
  auto* comp =
      new component::ComponentMock(component::Component::Type::Metadata);
  std::vector<component::Component*> components;
  node.AddComponent(comp);
  node.GetComponents(components);
  EXPECT_THAT(components, testing::ElementsAre(
                              transformComp));  // will be added after update

  node.RemoveComponent(comp);
  node.Update();

  components.clear();
  node.GetComponents(components);
  EXPECT_THAT(components, testing::ElementsAre(transformComp));
}

TEST_F(NodeTest, Update) {
  auto node = NodeMock();
  ASSERT_EQ(node.UpdateDirtyCalledCount, 0);

  node.Update();
  EXPECT_EQ(node.UpdateDirtyCalledCount, 0) << "after update";

  node.SetDirty(Node::DirtyImpact::Self);
  node.Update();
  EXPECT_EQ(node.UpdateDirtyCalledCount, 1) << "after dirty update";
}

TEST_F(NodeTest, UpdateDirtySelf) {
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
*/
std::bitset<4> toBitset(std::vector<Node::DirtyImpact> impacts) {
  auto bits = std::bitset<4>();
  for (auto impact : impacts) {
    bits[static_cast<std::int8_t>(impact)] = true;
  }
  return bits;
}
TEST_F(NodeTest, UpdateDirtyDependents) {
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

TEST_F(NodeTest, UpdateDirtyTransform) {
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

TEST_F(NodeTest, UpdateAlwaysUpdateComponent) {
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

TEST_F(NodeTest, HandleTransformChanges) {
  auto node = NodeMock();
  ASSERT_EQ(node.GetState(), Node::State::Normal);

  node.Transform().SetPosition(glm::vec3(1.F));
  EXPECT_EQ(node.GetState(), Node::State::Dirty);
  EXPECT_TRUE(node.IsDirtyImpact(Node::DirtyImpact::Transform));
}
/*
TEST_F(NodeTest, HandleComponentEventStateFilterOnNew) {
  auto eventListener = NodeEventMockListener();
  auto node = NodeMock();
  node.AddListener(&eventListener);
  auto* comp = node.AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));
  ASSERT_THAT(
      eventListener.Events,
      testing::ElementsAre(event::Node(&node, event::Node::ChangeType::State)));
  eventListener.ResetMocks();

  // comp->SetDirty();

  EXPECT_TRUE(eventListener.Events.empty());

  node.Update();
  // ASSERT_FALSE(comp->IsDirty());
  ASSERT_THAT(eventListener.Events,
              testing::ElementsAre(
                  event::Node::MakeComponentEvent(
                      &node, event::Component(
                                 comp, event::Component::TriggerType::Added)),
                  event::Node(&node, event::Node::ChangeType::State)));
  eventListener.ResetMocks();

  // comp->SetDirty();

  EXPECT_THAT(eventListener.Events,
              testing::ElementsAre(
                  event::Node(&node, event::Node::ChangeType::State),
                  event::Node::MakeComponentEvent(
                      &node, event::Component::MakeDataChangedEvent(comp))));
}
*/
TEST_F(NodeTest, GetComponents) {
  auto node = NodeMock();
  auto transformComp =
      dynamic_cast<const component::Component*>(&node.Transform());
  auto* comp = node.AddComponent(
      new component::ComponentMock(component::Component::Type::Metadata));

  std::vector<component::Component*> comps;

  node.GetComponents(comps);

  EXPECT_THAT(comps, testing::ElementsAre(transformComp));
  node.Update();

  comps.clear();
  node.GetComponents(comps);
  EXPECT_THAT(comps, testing::ElementsAre(transformComp, comp));

  comps.clear();
  node.GetComponents(comps, component::Component::Type::WorldEntity);
  EXPECT_TRUE(comps.empty());

  node.GetComponents(comps, component::Component::Type::Metadata);
  EXPECT_THAT(comps, testing::ElementsAre(comp));
}

TEST_F(NodeTest, ForEach) {
  Node* root = new NodeMock();
  Node* currentNode = root;
  std::chrono::duration<long, std::ratio<1, 1000000000>> diffItr{};
  /*std::chrono::duration<long, std::ratio<1, 1000000000>> diffRec {};
  std::chrono::duration<long, std::ratio<1, 1000000000>> diffItr2 {};
  std::chrono::duration<long, std::ratio<1, 1000000000>> diffItr3 {};*/
  std::chrono::duration<long, std::ratio<1, 1000000000>> diffItr2{};
  std::vector<NodeMock*> nodes;
  nodes.reserve(4500);
  for (auto i = 0; i < 1500; i++) {
    currentNode = currentNode->AddChild(new NodeMock(Node::Type::Game));
    nodes.push_back(dynamic_cast<NodeMock*>(currentNode));
    nodes.push_back(currentNode->AddChild(new NodeMock(Node::Type::Game)));
    nodes.push_back(currentNode->AddChild(new NodeMock(Node::Type::Game)));
  }
  constexpr auto probes = 10;
  for (auto probe = 0; probe < probes; probe++) {
    auto begin = std::chrono::steady_clock::now();
    Node::ForEachChild(root, [](Node* child) {});
    diffItr += std::chrono::steady_clock::now() - begin;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    /*begin = std::chrono::steady_clock::now();
    Scene::ForEachChild2(scene, [](Node* child) {});
    diffItr2 += std::chrono::steady_clock::now() - begin;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));*/
    /*
     begin = std::chrono::steady_clock::now();
     scene.ForEachChild3(&scene, [](Node*) {});
     diffItr2 += std::chrono::steady_clock::now() - begin;
     std::this_thread::sleep_for(std::chrono::milliseconds(1));

     begin = std::chrono::steady_clock::now();
     scene.ForEachChild4(&scene, [](Node*) {});
     diffItr3 += std::chrono::steady_clock::now() - begin;
     std::this_thread::sleep_for(std::chrono::milliseconds(1));*/
  }
  /*   for (const auto* child : nodes) {
         if (child->UpdateCalledCount != probes * 2) {
             printf("g");
         }
         ASSERT_EQ(child->UpdateCalledCount, probes * 2);
     }*/

  std::cout
      << "### Iterative: "
      << std::chrono::duration_cast<std::chrono::microseconds>(diffItr).count()
      << " ns"
      << " ###" << std::endl;
  std::cout
      << "### Iterative2: "
      << std::chrono::duration_cast<std::chrono::microseconds>(diffItr2).count()
      << " ns"
      << " ###" << std::endl;
  /*  std::cout << "### Iterative3: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(diffItr3).count()
              << " ns"
              << " ###" << std::endl;
    std::cout << "### Recursive: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(diffRec).count()
              << " ns"
              << " ###" << std::endl;*/
  delete root;
}

TEST_F(NodeTest, ForEachChild) {
  auto root = new NodeMock();
  std::vector<NodeMock*> children;

  auto child1000 = root->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child1000);
  auto child2000 = root->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2000);
  auto child3000 = root->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child3000);

  auto child1100 = child1000->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child1100);
  auto child1200 = child1000->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child1200);
  auto child1300 = child1000->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child1300);

  auto child2100 = child2000->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2100);
  auto child2200 = child2000->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2200);

  auto child2201 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2201);
  auto child2202 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2202);
  auto child2203 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2203);
  auto child2204 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2204);
  auto child2205 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2205);
  auto child2206 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2206);
  auto child2207 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2207);
  auto child2208 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2208);
  auto child2209 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2209);
  auto child2210 = child2100->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2210);

  auto child2300 = child2000->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2300);
  auto child2301 = child2300->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child2301);

  auto child3100 = child3000->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child3100);
  auto child3200 = child3000->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child3200);
  auto child3300 = child3000->AddChild(new NodeMock(Node::Type::Game));
  children.push_back(child3300);

  Node::ForEachChild(root, [](Node* child) { child->Update(); });

  for (const auto* child : children) {
    if (child->UpdateCalledCount == 0) {
      printf("g");
    }
    ASSERT_EQ(child->UpdateCalledCount, 1);
  }

  delete root;
}

}  // namespace soil::stage::scene
