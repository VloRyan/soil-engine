#include "stage/scene/gui/container/h_box.h"

#include "gtest/gtest.h"
#include "stage/scene/gui/rectangle.h"
#include "testing.h"

namespace soil::stage::scene::gui::container {
class HBoxTest : public testing::Test {
 protected:
  class HBoxSpy : public HBox {
   public:
    void Layout() override { HBox::Layout(); }

    void UpdateSize(const glm::ivec2& maxSize) override { HBox::UpdateSize(maxSize); }
  };
};

TEST_F(HBoxTest, Contructor) {
  const auto defaultBox = HBox();

  EXPECT_EQ(defaultBox.GetMargin(), 0);
  auto defaultAlignment = layout::Alignment{layout::Alignment::Horizontal::Center, layout::Alignment::Vertical::Center};
  EXPECT_EQ(defaultBox.GetItemAlignment(), defaultAlignment);
  EXPECT_VEC_EQ(defaultBox.GetChildrenSize(), glm::ivec2(0));
  EXPECT_VEC_EQ(defaultBox.GetPadding(), glm::ivec4(0));
  EXPECT_VEC_EQ(defaultBox.GetPosition(), glm::vec3(0.F, 0.F, 0.F));

  const auto box = HBox(2, glm::ivec4(1));
  EXPECT_VEC_EQ(box.GetPadding(), glm::ivec4(1));
  EXPECT_EQ(box.GetMargin(), 2);
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(0));
}

TEST_F(HBoxTest, Layout) {
  auto box = HBoxSpy();
  box.SetItemAlignment({.X=layout::Alignment::Horizontal::Left, .Y=layout::Alignment::Vertical::Center});
  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.SetSize(glm::ivec2(1000));
  box.SetSizeType(Rectangle::SizeTypes::Fixed);
  box.UpdateSize(glm::ivec2(1000));

  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(1000));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(), glm::vec3(-450.F, 0.F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({layout::Alignment::Horizontal::Center, layout::Alignment::Vertical::Top});
  box.Layout();
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(), glm::vec3(0.F, 450.F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({layout::Alignment::Horizontal::Right, layout::Alignment::Vertical::Bottom});
  box.Layout();
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(), glm::vec3(450.F, -450.F, Rectangle::LAYER_Z_INCREMENT));
}

TEST_F(HBoxTest, LayoutWithGrow) {
  auto box = HBoxSpy();
  box.SetItemAlignment({.X=layout::Alignment::Horizontal::Left, .Y=layout::Alignment::Vertical::Center});

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));

  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(100));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(),
                glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_INCREMENT));

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(300, 100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(300, 100));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(),
                glm::vec3(-100.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(),
                glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(2)->GetPosition(),
                glm::vec3(100.F, 0.F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({.X=layout::Alignment::Horizontal::Left, .Y=layout::Alignment::Vertical::Top});
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(300, 100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(300, 100));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(),
                glm::vec3(-100.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(),
                glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(2)->GetPosition(),
                glm::vec3(100.F, 0.F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({.X=layout::Alignment::Horizontal::Left, .Y=layout::Alignment::Vertical::Bottom});
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(300, 100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(300, 100));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(),
                glm::vec3(-100.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(),
                glm::vec3(0.F, -0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(2)->GetPosition(),
                glm::vec3(100.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
}

TEST_F(HBoxTest, LayoutWithRelativedSize) {
  auto box = HBoxSpy();
  box.SetSizeType(Base::SizeTypes::Relative);
  box.SetRelativeSize(glm::vec2(0.5F));
  box.SetItemAlignment({.X=layout::Alignment::Horizontal::Left, .Y=layout::Alignment::Vertical::Center});
  auto* child = box.AddChild(new Rectangle(glm::ivec2(100)));
  auto* child2 = box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(800));

  box.SetPadding(glm::ivec4(5, 10, 15, 20));
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(200, 100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(400, 400));
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(-145.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child2->GetPosition(),
                glm::vec3(-45.F, 0.F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({.X=layout::Alignment::Horizontal::Left, .Y=layout::Alignment::Vertical::Top});
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(200, 100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(400, 400));
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(-145.F, 140.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child2->GetPosition(),
                glm::vec3(-45.F, 140.F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({.X=layout::Alignment::Horizontal::Left, .Y=layout::Alignment::Vertical::Bottom});
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(200, 100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(400, 400));
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(-145.F, -130.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child2->GetPosition(),
                glm::vec3(-45.F, -130.F, Rectangle::LAYER_Z_INCREMENT));
}

TEST_F(HBoxTest, LayoutWithMargin) {
  auto box = HBoxSpy();
  box.SetSize(glm::ivec2(400, 400));
  box.SetSizeType(Base::SizeTypes::Fixed);
  box.SetItemAlignment({.X=layout::Alignment::Horizontal::Left, .Y=layout::Alignment::Vertical::Center});
  box.SetMargin(10);
  auto* child = box.AddChild(new Rectangle(glm::ivec2(100)));
  auto* child2 = box.AddChild(new Rectangle(glm::ivec2(100)));

  box.UpdateSize(glm::ivec2(800));
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(210, 100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(400, 400));
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(-150.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child2->GetPosition(),
                glm::vec3(-40.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
}

TEST_F(HBoxTest, UpdateSizeWithGrow) {
  auto box = HBoxSpy();
  box.SetSizeType(Base::SizeTypes::GrowWithContent);

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(100, 100));

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(200, 100));

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(300, 100));

  box.SetMargin(10);
  box.UpdateSize(glm::ivec2(1000));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(320, 100));
}
}  // namespace soil::stage::scene::gui::container
