#include "stage/scene/render/instancing.h"

#include <gtest/gtest.h>

#include <utility>

#include "mocks.hpp"
namespace soil::stage::scene::render {
class InstancingTest : public testing::Test {};
class InstanceMock : public component::InstanceData {
 public:
  InstanceMock(std::string batchKey, bool opaque)
      : InstanceData(std::move(batchKey), opaque) {}
  void WriteData(video::buffer::Cursor* cursor) const override {}
};

class BatchObjectMock : public Instancing::BatchObject {
 public:
  explicit BatchObjectMock() : Instancing::BatchObject() {}
  void AddChangedInstance(video::render::instance::Instance* instance) {
    Calls.AddChangedInstance++;
  }

  void AddNewInstance(video::render::instance::Instance* instance) override {
    Calls.AddNewInstance++;
  }

  bool RemoveInstance(video::render::instance::Instance* instance) override {
    Calls.RemoveInstance++;
    return true;
  }
  void Update() override { Calls.Update++; }
  struct Calls_t {
    int AddChangedInstance{0};
    int AddNewInstance{0};
    int RemoveInstance{0};
    int Update{0};
  } Calls;
};
static Instancing::BatchObject* CreateBatchObjectMock(
    const Instancing::BatchObjectCreationArgs&) {
  return new BatchObjectMock();
}

TEST_F(InstancingTest, AddRenderBatch) {
  auto renderContainer = video::render::Container();
  auto instancing = Instancing(&renderContainer);
  auto meshData = video::mesh::Data();

  EXPECT_THROW(instancing.AddRenderBatch("data", {}), std::invalid_argument);

  EXPECT_NO_THROW(instancing.AddRenderBatch("data", {
                                                        .Mesh = &meshData,
                                                    }));
  EXPECT_TRUE(instancing.GetStateBatches("data") != nullptr);
  EXPECT_TRUE(instancing.GetStateBatches("atad") == nullptr);
}

TEST_F(InstancingTest, OnAdded) {
  auto renderContainer = video::render::Container();
  auto instancing = Instancing(&renderContainer, CreateBatchObjectMock);
  auto component = InstanceMock("data", true);
  auto meshData = video::mesh::Data();
  instancing.AddRenderBatch("data", {
                                        .Mesh = &meshData,
                                    });
  auto dataStateBatches = instancing.GetStateBatches("data");
  ASSERT_TRUE(dataStateBatches != nullptr);
  ASSERT_TRUE(dataStateBatches->Opaque == nullptr);

  instancing.Handle(
      event::Component(&component, event::Component::TriggerType::Added));

  EXPECT_TRUE(renderContainer.Empty());
  EXPECT_TRUE(dataStateBatches->Opaque == nullptr);  // still in "added" state

  // Simulate render
  auto emptyState = video::render::State{};
  instancing.OnTrigger(hook::TriggerHook::TriggerType::AfterUpdateScene);
  component.SetIndex(1);

  auto* batchMock = dynamic_cast<BatchObjectMock*>(dataStateBatches->Opaque);
  ASSERT_TRUE(batchMock != nullptr);
  EXPECT_EQ(batchMock->Calls.AddNewInstance, 1);
}

TEST_F(InstancingTest, OnChanged) {
  auto renderContainer = RenderContainerMock();
  auto instancing = Instancing(&renderContainer, CreateBatchObjectMock);
  auto component = InstanceMock("data", true);
  auto meshData = video::mesh::Data();
  instancing.AddRenderBatch("data", {
                                        .Mesh = &meshData,
                                    });
  instancing.Handle(
      event::Component(&component, event::Component::TriggerType::Added));
  // Simulate render
  auto emptyState = video::render::State{};
  instancing.OnTrigger(hook::TriggerHook::TriggerType::AfterUpdateScene);
  component.SetIndex(1);
  ASSERT_EQ(renderContainer.Calls.Add, 1);

  instancing.Handle(event::Component(&component,
                                     event::Component::TriggerType::Changed,
                                     event::Component::ChangeType::Data));
  EXPECT_EQ(renderContainer.Calls.Add, 1);
}

TEST_F(InstancingTest, OnChangedWithoutAdded) {
  auto renderContainer = RenderContainerMock();
  auto instancing = Instancing(&renderContainer, CreateBatchObjectMock);
  auto component = InstanceMock("data", true);
  auto meshData = video::mesh::Data();
  instancing.AddRenderBatch("data", {
                                        .Mesh = &meshData,
                                    });
  instancing.Handle(event::Component(&component,
                                     event::Component::TriggerType::Changed,
                                     event::Component::ChangeType::Data));

  EXPECT_EQ(renderContainer.Calls.Add, 0);  //  no effect

  // Simulate render
  auto emptyState = video::render::State{};
  instancing.OnTrigger(hook::TriggerHook::TriggerType::AfterUpdateScene);
  component.SetIndex(1);

  EXPECT_EQ(renderContainer.Calls.Add, 1);
}

TEST_F(InstancingTest, OnChangedBeforeFirstRender) {
  auto renderContainer = RenderContainerMock();
  auto instancing = Instancing(&renderContainer, CreateBatchObjectMock);
  auto component = InstanceMock("data", true);
  auto meshData = video::mesh::Data();
  instancing.AddRenderBatch("data", {
                                        .Mesh = &meshData,
                                    });
  instancing.Handle(
      event::Component(&component, event::Component::TriggerType::Added));

  EXPECT_EQ(renderContainer.Calls.Add, 0);  //  no effect

  component.SetVisible(false);
  component.Update();  // reset dirty flag
  instancing.Handle(event::Component(&component,
                                     event::Component::TriggerType::Changed,
                                     event::Component::ChangeType::Data));

  // Simulate render
  auto emptyState = video::render::State{};
  instancing.OnTrigger(hook::TriggerHook::TriggerType::AfterUpdateScene);
  component.SetIndex(1);

  EXPECT_EQ(renderContainer.Calls.Add, 0);
}

}  // namespace soil::stage::scene::render
