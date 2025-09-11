#include "stage/scene/gui/container/h_box.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "stage/scene/gui/container/v_box.h"
#include "stage/scene/gui/rectangle.h"
#include "testing.h"

namespace soil::stage::scene::gui::container {
    class HBoxTest : public testing::Test {};

    TEST_F(HBoxTest, Contructor) {
        const auto defaultBox = HBox();

        EXPECT_EQ(defaultBox.GetMargin(), 0);
        EXPECT_EQ(defaultBox.GetAlignItems(), HBox::AlignItems::Center);
        EXPECT_VEC_EQ(defaultBox.GetItemsSize(), glm::ivec2(0));
        EXPECT_VEC_EQ(defaultBox.GetPadding(), glm::ivec4(0));

        const auto box = HBox(2, glm::ivec4(1));
        EXPECT_VEC_EQ(box.GetPadding(), glm::ivec4(1));
        EXPECT_EQ(box.GetMargin(), 2);
        EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(0));
    }

    TEST_F(HBoxTest, Update) {
        auto box = HBox();
        box.SetSize(glm::ivec2(800, 600));

        auto* child = box.AddChild(new Rectangle());
        child->SetSize(glm::ivec2(100));

        box.Update();
        EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(100));
        EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600))
        EXPECT_VEC_EQ(child->GetPosition(), glm::vec3(-350.F, 0.F, Rectangle::LAYER_Z_INCREMENT));

        auto* child2 = box.AddChild(new Rectangle());
        child2->SetSize(glm::ivec2(100));

        box.Update();
        EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(200, 100));
        EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600));
        EXPECT_VEC_EQ(child->GetPosition(), glm::vec3(-350.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
        EXPECT_VEC_EQ(child2->GetPosition(), glm::vec3(-250.F, 0.F, Rectangle::LAYER_Z_INCREMENT));

        auto* child3 = box.AddChild(new Rectangle());
        child3->SetSize(glm::ivec2(150));

        constexpr auto margin = 10;
        box.SetMargin(margin);
        box.Update();
        EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(350 + 2 * margin, 150));
        EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600));
        EXPECT_VEC_EQ(child->GetPosition(), glm::vec3(-350.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
        EXPECT_VEC_EQ(child2->GetPosition(), glm::vec3(-250.F + margin, 0.F, Rectangle::LAYER_Z_INCREMENT));
        EXPECT_VEC_EQ(child3->GetPosition(), glm::vec3(-125.F + 2 * margin, 0.F, Rectangle::LAYER_Z_INCREMENT));

        constexpr auto offset = 20;
        box.SetOffset(glm::ivec2(offset, 0));
        box.Update();
        EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(350 + 2 * margin, 150));
        EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600));
        EXPECT_VEC_EQ(child->GetPosition(), glm::vec3(-350.F + offset, 0.F, Rectangle::LAYER_Z_INCREMENT));
        EXPECT_VEC_EQ(child2->GetPosition(), glm::vec3(-250.F + offset + margin, 0.F, Rectangle::LAYER_Z_INCREMENT));
        EXPECT_VEC_EQ(child3->GetPosition(),
                      glm::vec3(-125.F + offset + 2 * margin, 0.F, Rectangle::LAYER_Z_INCREMENT));
    }

    TEST_F(HBoxTest, UpdateAlign) {
        auto box = HBox();
        box.SetSize(glm::ivec2(800, 600));
        box.SetPadding(glm::ivec4(0, 10, 0, 20));
        box.SetAlignItems(HBox::AlignItems::Top);

        auto* child = box.AddChild(new Rectangle());
        child->SetSize(glm::ivec2(100));

        box.Update();
        EXPECT_VEC_EQ(box.GetItemsSize(), glm::ivec2(100));
        EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(800, 600))
        EXPECT_VEC_EQ(child->GetPosition(), glm::vec3(-350.F, 240.F, Rectangle::LAYER_Z_INCREMENT));

        box.SetAlignItems(HBox::AlignItems::Center);
        box.Update();
        EXPECT_VEC_EQ(child->GetPosition(), glm::vec3(-350.F, -10.F, Rectangle::LAYER_Z_INCREMENT));

        box.SetAlignItems(HBox::AlignItems::Bottom);
        box.Update();
        EXPECT_VEC_EQ(child->GetPosition(), glm::vec3(-350.F, -230.F, Rectangle::LAYER_Z_INCREMENT));
    }

} // namespace soil::stage::scene::gui::container
