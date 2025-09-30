#include "stage/scene/gui/container/v_box.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "stage/scene/gui/rectangle.h"
#include "testing.h"

namespace soil::stage::scene::gui::container {
class VBoxTest : public testing::Test {};

TEST_F(VBoxTest, Contructor) {
  const auto defaultBox = VBox();

  EXPECT_EQ(defaultBox.GetAlignItems(), VBox::AlignItems::Center);
  EXPECT_EQ(defaultBox.GetMargin(), 0);
  EXPECT_VEC_EQ(defaultBox.GetPadding(), glm::ivec4(0));
  EXPECT_VEC_EQ(defaultBox.GetItemsSize(), glm::ivec2(0));

  const auto box = VBox(2, glm::ivec4(1));
  EXPECT_VEC_EQ(box.GetPadding(), glm::ivec4(1));
  EXPECT_EQ(box.GetMargin(), 2);
  EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(0));
}

TEST_F(VBoxTest, Update) {
  auto box = VBox();
  box.SetSize(glm::ivec2(800, 600));

  auto* child = box.AddChild(new Rectangle());
  child->SetSize(glm::ivec2(100));

  box.Update();
  EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600))
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(0.F, 250.F, Rectangle::LAYER_Z_INCREMENT));

  auto* child2 = box.AddChild(new Rectangle());
  child2->SetSize(glm::ivec2(100));

  box.Update();
  EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(100, 200));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600));
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(0.F, 250.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child2->GetPosition(),
                glm::vec3(0.F, 150.F, Rectangle::LAYER_Z_INCREMENT));

  auto* child3 = box.AddChild(new Rectangle());
  child3->SetSize(glm::ivec2(150));

  constexpr auto margin = 10;
  box.SetMargin(margin);
  box.Update();
  EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(150, 350 + 2 * margin));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600));
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(0.F, 250.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child2->GetPosition(),
                glm::vec3(0.F, 150.F - margin, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child3->GetPosition(), glm::vec3(0.F, 25.F - 2 * margin,
                                                 Rectangle::LAYER_Z_INCREMENT));

  constexpr auto offset = 20;
  box.SetOffset(glm::ivec2(0, offset));
  box.Update();
  EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(150, 350 + 2 * margin));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600));
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(0.F, 250.F + offset, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child2->GetPosition(), glm::vec3(0.F, 150.F + offset - margin,
                                                 Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(
      child3->GetPosition(),
      glm::vec3(0.F, 25.F + offset - 2 * margin, Rectangle::LAYER_Z_INCREMENT));
}

TEST_F(VBoxTest, UpdateAlign) {
  auto box = VBox();
  box.SetSize(glm::ivec2(800, 600));
  box.SetPadding(glm::ivec4(10, 0, 20, 0));
  box.SetAlignItems(VBox::AlignItems::Left);

  auto* child = box.AddChild(new Rectangle());
  child->SetSize(glm::ivec2(100));

  box.Update();
  EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600))
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(-340.F, 250.F, Rectangle::LAYER_Z_INCREMENT));

  box.SetAlignItems(VBox::AlignItems::Center);
  box.Update();
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(-10.F, 250.F, Rectangle::LAYER_Z_INCREMENT));

  box.SetAlignItems(VBox::AlignItems::Right);
  box.Update();
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(330.F, 250.F, Rectangle::LAYER_Z_INCREMENT));
}

}  // namespace soil::stage::scene::gui::container
