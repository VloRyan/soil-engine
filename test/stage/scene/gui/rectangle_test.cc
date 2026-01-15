#include "stage/scene/gui/rectangle.h"

#include <gmock/gmock-matchers.h>

#include "gtest/gtest.h"
#include "stage/scene/gui/root.h"
#include "testing.h"
#include "util/strings.h"

namespace soil::stage::scene::gui {
class RectangleTest : public testing::Test {
 protected:
  static inline float LAYER_Z_BOTTOM =
      -Rectangle::TOP_Z_LAYER + Rectangle::LAYER_Z_INCREMENT;
  class RectSpy : public Rectangle {
   public:
    RectSpy(const glm::ivec2 size = glm::ivec2(0)) : Rectangle(size) {}
    const std::vector<Rectangle*>& ChildRects() const { return childRects_; }
    void Layout() override { Rectangle::Layout(); };
    void UpdateScissor(const video::render::Rect& parentRect) override {
      Rectangle::UpdateScissor(parentRect);
    };
    video::render::Rect CalculateChildScissorRect() const override {
      return Rectangle::CalculateChildScissorRect();
    }
  };
};

TEST_F(RectangleTest, Contruct) {
  const auto rect = Rectangle();

  EXPECT_VEC_EQ(rect.GetPosition(), glm::vec3(0.F));
  EXPECT_VEC_EQ(rect.GetRelativeSize(), glm::vec2(0.F));
  EXPECT_EQ(rect.GetAspectRatio(), 0.F);
  EXPECT_VEC_EQ(rect.GetPadding(), glm::ivec4(0));
  EXPECT_VEC_EQ(rect.GetSize(), glm::ivec2(0));
  EXPECT_VEC_EQ(rect.GetChildrenSize(), glm::ivec2(0));
  EXPECT_VEC_EQ(rect.GetMaxSize(), glm::ivec2(std::numeric_limits<int>::max()));
  EXPECT_VEC_EQ(rect.GetMinSize(), glm::ivec2(0));
  EXPECT_EQ(rect.GetSizeType(), Rectangle::SizeTypes::Fixed);
}

TEST_F(RectangleTest, UpdateScissor) {
  auto parentRect = video::render::Rect{
      .LowerLeftPosition = glm::ivec2(0),
      .Size = glm::ivec2(800, 600),
  };
  auto rect = RectSpy(glm::ivec2(400, 400));
  rect.SetPosition(glm::vec3(0.F, 0.F, 0.F));

  rect.UpdateScissor(parentRect);
  EXPECT_VEC_EQ(rect.GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0));
  EXPECT_VEC_EQ(rect.GetScissorRect().Size, glm::ivec2(200, 200));

  rect.SetPosition(glm::vec3(0.F, 200.F, 0.F));
  rect.UpdateScissor(parentRect);
  EXPECT_VEC_EQ(rect.GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0));
  EXPECT_VEC_EQ(rect.GetScissorRect().Size, glm::ivec2(200, 400));

  rect.SetPosition(glm::vec3(200.F, 200.F, 0.F));
  rect.UpdateScissor(parentRect);
  EXPECT_VEC_EQ(rect.GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0));
  EXPECT_VEC_EQ(rect.GetScissorRect().Size, glm::ivec2(400, 400));

  rect.SetPosition(glm::vec3(400.F, 500.F, 0.F));
  rect.UpdateScissor(parentRect);
  EXPECT_VEC_EQ(rect.GetScissorRect().LowerLeftPosition, glm::ivec2(200, 300));
  EXPECT_VEC_EQ(rect.GetScissorRect().Size, glm::ivec2(400, 300));
}

TEST_F(RectangleTest, UpdateScissorWithPadding) {
  auto parentRect = video::render::Rect{
      .LowerLeftPosition = glm::ivec2(-100),
      .Size = glm::ivec2(200),
  };

  auto rect = RectSpy(glm::vec2(100, 100));

  const auto childRect = rect.AddChild(new RectSpy(glm::vec2(94, 94)));
  rect.UpdateScissor(parentRect);
  childRect->UpdateScissor(rect.CalculateChildScissorRect());
  EXPECT_VEC_EQ(rect.GetScissorRect().LowerLeftPosition, glm::ivec2(-50));
  EXPECT_VEC_EQ(rect.GetScissorRect().Size, glm::ivec2(100));
  EXPECT_VEC_EQ(childRect->GetScissorRect().LowerLeftPosition, glm::ivec2(-47));
  EXPECT_VEC_EQ(childRect->GetScissorRect().Size, glm::ivec2(94));

  rect.SetPadding(glm::vec4(5, 10, 15, 20));
  rect.UpdateScissor(parentRect);
  childRect->UpdateScissor(rect.CalculateChildScissorRect());
  EXPECT_VEC_EQ(rect.GetScissorRect().LowerLeftPosition, glm::ivec2(-50));
  EXPECT_VEC_EQ(rect.GetScissorRect().Size, glm::ivec2(100));
  EXPECT_VEC_EQ(childRect->GetScissorRect().LowerLeftPosition,
                glm::ivec2(-45, -30));
  EXPECT_VEC_EQ(childRect->GetScissorRect().Size, glm::ivec2(80, 70));
}

TEST_F(RectangleTest, UpdateVisibility) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());

  EXPECT_EQ(root.IsVisible(), true);
  EXPECT_EQ(rect->IsVisible(), true);

  root.SetVisible(false);
  root.Update();
  EXPECT_EQ(root.IsVisible(), false);
  EXPECT_EQ(rect->IsVisible(), false);

  rect->SetVisible(true);
  root.Update();
  EXPECT_EQ(root.IsVisible(), false);
  EXPECT_EQ(rect->IsVisible(), false);

  root.SetVisible(true);
  root.Update();
  EXPECT_EQ(root.IsVisible(), true);
  EXPECT_EQ(rect->IsVisible(), true);

  rect->SetVisible(false);
  rect->Update();
  EXPECT_EQ(root.IsVisible(), true);
  EXPECT_EQ(rect->IsVisible(), false);
}

TEST_F(RectangleTest, UpdateSizeWithPadding) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());

  rect->SetRelativeSize(glm::vec2(0.5F, 0.5F));
  root.Update();
  EXPECT_VEC_EQ(rect->GetSize(), glm::ivec2(400, 300));

  root.SetPadding(glm::ivec4(50));
  root.Update();
  EXPECT_VEC_EQ(rect->GetSize(), glm::ivec2(350, 250));
}

TEST_F(RectangleTest, UpdateSizeWithAspectRatio) {
  auto rect = Rectangle();

  rect.SetRelativeSize(glm::vec2(1.F, 0.F));
  rect.SetAspectRatio(2.F / 1.F);
  rect.UpdateSize(glm::ivec2(100, 100));
  EXPECT_VEC_EQ(rect.GetSize(), glm::ivec2(100, 50));

  rect.SetRelativeSize(glm::vec2(0.F, 1.F));
  rect.SetAspectRatio(2.F / 1.F);
  rect.UpdateSize(glm::ivec2(100, 100));
  EXPECT_VEC_EQ(rect.GetSize(), glm::ivec2(200, 100));
}

TEST_F(RectangleTest, SetParent) {
  auto rect = RectSpy();
  auto childRect = rect.AddChild(new Rectangle());

  EXPECT_THAT(rect.ChildRects(), testing::ElementsAre(childRect));
  EXPECT_EQ(childRect->GetParentRect(), &rect);

  childRect->SetParent(nullptr);
  EXPECT_TRUE(rect.ChildRects().empty());
  EXPECT_EQ(childRect->GetParentRect(), nullptr);

  childRect->SetParent(&rect);
  EXPECT_THAT(rect.ChildRects(), testing::ElementsAre(childRect));
  EXPECT_EQ(childRect->GetParentRect(), &rect);
}

TEST_F(RectangleTest, GuiRoot) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());
  const auto childRect = rect->AddChild(new Rectangle());
  const auto nonRootedRect = Rectangle();
  const auto derivedRect = root.AddChild(new RectSpy());

  EXPECT_EQ(rect->GuiRoot(), &root);
  EXPECT_EQ(childRect->GuiRoot(), &root);
  EXPECT_EQ(derivedRect->GuiRoot(), &root);
  EXPECT_EQ(nonRootedRect.GuiRoot(), nullptr);
}

TEST_F(RectangleTest, Layout) {
  auto rect = RectSpy();
  const auto childRect = rect.AddChild(new Rectangle());
  const auto childRect2 = rect.AddChild(new Rectangle());

  EXPECT_VEC_EQ(childRect->GetPosition(),
                glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(childRect2->GetPosition(),
                glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_INCREMENT));

  rect.Layout();
  EXPECT_VEC_EQ(childRect->GetPosition(),
                glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(childRect2->GetPosition(),
                glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
}
}  // namespace soil::stage::scene::gui
