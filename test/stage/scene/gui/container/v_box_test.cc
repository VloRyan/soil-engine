#include "stage/scene/gui/container/v_box.h"

#include "gtest/gtest.h"
#include "stage/scene/gui/rectangle.h"
#include "testing.h"

namespace soil::stage::scene::gui::container {
class VBoxTest : public testing::Test {
 protected:
  class VBoxSpy : public VBox {
   public:
    void Layout() override { VBox::Layout(); }

    void UpdateDirty() override { VBox::UpdateDirty(); }

    void UpdateScissor(const video::render::Rect& parentRect) override { VBox::UpdateScissor(parentRect); }

    void UpdateSize(const glm::ivec2& maxSize) override { VBox::UpdateSize(maxSize); }
  };
};

TEST_F(VBoxTest, Contructor) {
  const auto defaultBox = VBox();
  auto defaultAlignment = layout::Alignment{.X =  layout::Alignment::Horizontal::Center,
      .Y=layout::Alignment::Vertical::Center};
  EXPECT_EQ(defaultBox.GetItemAlignment(), defaultAlignment);
  EXPECT_EQ(defaultBox.GetMargin(), 0);
  EXPECT_VEC_EQ(defaultBox.GetPadding(), glm::ivec4(0));
  EXPECT_VEC_EQ(defaultBox.GetChildrenSize(), glm::ivec2(0));

  const auto box = VBox(2, glm::ivec4(1));
  EXPECT_VEC_EQ(box.GetPadding(), glm::ivec4(1));
  EXPECT_EQ(box.GetMargin(), 2);
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(0));
}

TEST_F(VBoxTest, Layout) {
  auto box = VBoxSpy();
  box.SetSize(glm::ivec2(1000));
  box.SetSizeType(Base::SizeTypes::Fixed);
  box.SetItemAlignment({layout::Alignment::Horizontal::Center, layout::Alignment::Vertical::Top});
  box.UpdateSize(glm::ivec2(1000));
  box.AddChild(new Rectangle(glm::ivec2(100)));

  box.Layout();
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(1000));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(), glm::vec3(0.F, 450.0F, Rectangle::LAYER_Z_INCREMENT));

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));
  box.Layout();
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(1000));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(), glm::vec3(0.F, 450.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(), glm::vec3(0.F, 350.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(2)->GetPosition(), glm::vec3(0.F, 250.0F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({layout::Alignment::Horizontal::Left, layout::Alignment::Vertical::Center});
  box.Layout();
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(1000));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(), glm::vec3(-450.F, 100.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(), glm::vec3(-450.F, 0.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(2)->GetPosition(), glm::vec3(-450.F, -100.0F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({layout::Alignment::Horizontal::Right, layout::Alignment::Vertical::Bottom});
  box.Layout();
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(1000));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(), glm::vec3(450.F, -250.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(), glm::vec3(450.F, -350.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(2)->GetPosition(), glm::vec3(450.F, -450.0F, Rectangle::LAYER_Z_INCREMENT));
}

TEST_F(VBoxTest, LayoutWithPadding) {
  auto box = VBoxSpy();
  box.SetSize(glm::ivec2(400, 400));
  box.SetSizeType(Base::SizeTypes::Fixed);
  box.SetItemAlignment({layout::Alignment::Horizontal::Center, layout::Alignment::Vertical::Top});
  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.AddChild(new VBox());
  box.Child(1)->SetRelativeSize(glm::vec2(0.25F)); // 95x95
  box.SetPadding(glm::ivec4(5, 12, 15, 8));
  box.UpdateSize(glm::ivec2(1000));

  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(100, 195));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(),
                glm::vec3(0.F, 138.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(),
                glm::vec3(0.F, 40.5F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({layout::Alignment::Horizontal::Left, layout::Alignment::Vertical::Top});
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(100, 195));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(),
                glm::vec3(-145.F, 138.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(),
                glm::vec3(-147.5F, 40.5F, Rectangle::LAYER_Z_INCREMENT));

  box.SetItemAlignment({layout::Alignment::Horizontal::Right, layout::Alignment::Vertical::Top});
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(100, 195));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(),
                glm::vec3(135.F, 138.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(),
                glm::vec3(137.5F, 40.5F, Rectangle::LAYER_Z_INCREMENT));
}

TEST_F(VBoxTest, LayoutWithMargin) {
  auto box = VBoxSpy();
  box.SetSize(glm::ivec2(400, 400));
  box.SetSizeType(Base::SizeTypes::Fixed);
  box.SetItemAlignment({layout::Alignment::Horizontal::Center, layout::Alignment::Vertical::Top});
  box.SetMargin(10);
  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));

  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(100, 210));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(400, 400));
  EXPECT_VEC_EQ(box.Child(0)->GetPosition(),
                glm::vec3(0.F, 150.0F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(box.Child(1)->GetPosition(),
                glm::vec3(0.F, 40.0F, Rectangle::LAYER_Z_INCREMENT));
}

TEST_F(VBoxTest, UpdateSizeWithGrow) {
  auto box = VBoxSpy();
  box.SetSizeType(Base::SizeTypes::GrowWithContent);

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(100, 100));

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(100, 200));

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(1000));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(100, 300));

  box.SetMargin(10);
  box.UpdateSize(glm::ivec2(1000));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(100, 320));
}

TEST_F(VBoxTest, UpdateScissor) {
  auto box = VBoxSpy();
  box.SetSize(glm::ivec2(100));
  box.SetItemAlignment({layout::Alignment::Horizontal::Center, layout::Alignment::Vertical::Top});
  box.SetPadding(glm::ivec4(10));
  box.AddChild(new Rectangle(glm::ivec2(100)));

  box.UpdateScissor({
                        .LowerLeftPosition = glm::ivec2(-400),
                        .Size = glm::ivec2(800),
                    });
  box.Update();

  EXPECT_VEC_EQ(box.GetScissorRect().Size, glm::ivec2(100));
  EXPECT_VEC_EQ(box.GetScissorRect().LowerLeftPosition, glm::ivec2(-50));
  EXPECT_VEC_EQ(box.Child(0)->GetScissorRect().Size, glm::ivec2(80));
  EXPECT_VEC_EQ(box.Child(0)->GetScissorRect().LowerLeftPosition,
                glm::ivec2(-40));
}
}  // namespace soil::stage::scene::gui::container
