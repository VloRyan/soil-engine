#include "stage/scene/gui/container/flow_box.h"

#include "gtest/gtest.h"
#include "testing.h"

namespace soil::stage::scene::gui::container {
class FlowBoxTest : public testing::Test {
 protected:
  class FlowBoxSpy : public FlowBox {
   public:
    void Layout() override { FlowBox::Layout(); }
    void UpdateSize(const glm::ivec2& maxSize) override { FlowBox::UpdateSize(maxSize); }
  };
};

TEST_F(FlowBoxTest, Contructor) {
  const auto defaultBox = FlowBox();

  EXPECT_EQ(defaultBox.GetMargin(), 0);
  EXPECT_VEC_EQ(defaultBox.GetChildrenSize(), glm::ivec2(0));
  EXPECT_VEC_EQ(defaultBox.GetPadding(), glm::ivec4(0));
  EXPECT_VEC_EQ(defaultBox.GetPosition(), glm::vec3(0.F, 0.F, 0.F));

  const auto box = FlowBox(2, glm::ivec4(1));
  EXPECT_VEC_EQ(box.GetPadding(), glm::ivec4(1));
  EXPECT_EQ(box.GetMargin(), 2);
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(0));
}

TEST_F(FlowBoxTest, LayoutWithGrow) {
  auto box = FlowBoxSpy();

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
}

TEST_F(FlowBoxTest, LayoutWithRelativedSize) {
  auto box = FlowBoxSpy();
  box.SetSizeType(Base::SizeTypes::Relative);
  box.SetRelativeSize(glm::vec2(0.5, 0.5));
  auto* child = box.AddChild(new Rectangle(glm::ivec2(100)));
  auto* child2 = box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(800));

  box.SetPadding(glm::ivec4(5, 10, 15, 20));
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(200, 100));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(400, 400));
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(-145.F, 140.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child2->GetPosition(),
                glm::vec3(-45.F, 140.F, Rectangle::LAYER_Z_INCREMENT));

}

TEST_F(FlowBoxTest, LayoutWithMargin) {
  auto box = FlowBoxSpy();
  box.SetSize(glm::ivec2(250, 250));
  box.SetSizeType(Base::SizeTypes::Fixed);
  box.SetMargin(10);
  auto* child = box.AddChild(new Rectangle(glm::ivec2(100)));
  auto* child2 = box.AddChild(new Rectangle(glm::ivec2(100)));
  auto* child3 = box.AddChild(new Rectangle(glm::ivec2(100)));

  box.UpdateSize(glm::ivec2(800));
  box.Layout();
  EXPECT_VEC_EQ(box.GetChildrenSize(), glm::ivec2(210, 210));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(250, 250));
  EXPECT_VEC_EQ(child->GetPosition(),
                glm::vec3(-75.F, 75.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child2->GetPosition(),
                glm::vec3(35.F, 75.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(child3->GetPosition(),
                glm::vec3(-75.F, -35.F, Rectangle::LAYER_Z_INCREMENT));
}

TEST_F(FlowBoxTest, UpdateSizeWithGrow) {
  auto box = FlowBoxSpy();
  box.SetSizeType(Base::SizeTypes::GrowWithContent);

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(310));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(100, 100));

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(310));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(200, 100));

  box.AddChild(new Rectangle(glm::ivec2(100)));
  box.UpdateSize(glm::ivec2(310));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(300, 100));

  box.SetMargin(10);
  box.UpdateSize(glm::ivec2(310));
  EXPECT_VEC_EQ(box.GetSize(), glm::ivec2(210, 210));
}
}  // namespace soil::stage::scene::gui::container
