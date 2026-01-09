#include "stage/scene/gui/rectangle.h"

#include "gtest/gtest.h"
#include "stage/scene/gui/root.h"
#include "testing.h"
#include "util/strings.h"

namespace soil::stage::scene::gui {
class RectangleTest : public testing::Test {
 protected:
  static inline float LAYER_Z_BOTTOM =
      -Rectangle::TOP_Z_LAYER + Rectangle::LAYER_Z_INCREMENT;
};

TEST_F(RectangleTest, Contruct) {
  const auto rect = Rectangle();

  EXPECT_VEC_EQ(rect.GetPosition(), glm::vec3(0.F, 0.F, 0.1F));
  EXPECT_VEC_EQ(rect.GetRelativeSize(), glm::vec2(0.F));
  EXPECT_VEC_EQ(rect.GetAspectRatio(), 0.F);
  EXPECT_VEC_EQ(rect.GetChildSize(), rect.GetSize());
}

TEST_F(RectangleTest, UpdateScissor) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());
  rect->SetSize(glm::vec2(400, 400));
  rect->SetPosition(glm::vec3(0.F, 0.F, 0.F));

  rect->Update();
  EXPECT_VEC_EQ(rect->GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0))
  EXPECT_VEC_EQ(rect->GetScissorRect().Size, glm::ivec2(200, 200))

  rect->SetPosition(glm::vec3(0.F, 200.F, 0.F));
  rect->Update();
  EXPECT_VEC_EQ(rect->GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0))
  EXPECT_VEC_EQ(rect->GetScissorRect().Size, glm::ivec2(200, 400))

  rect->SetPosition(glm::vec3(200.F, 200.F, 0.F));
  rect->Update();
  EXPECT_VEC_EQ(rect->GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0))
  EXPECT_VEC_EQ(rect->GetScissorRect().Size, glm::ivec2(400, 400))

  rect->SetPosition(glm::vec3(400.F, 500.F, 0.F));
  rect->Update();
  EXPECT_VEC_EQ(rect->GetScissorRect().LowerLeftPosition, glm::ivec2(200, 300))
  EXPECT_VEC_EQ(rect->GetScissorRect().Size, glm::ivec2(400, 300))
}

TEST_F(RectangleTest, UpdateScissorWithPadding) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());
  rect->SetPosition(glm::vec3(200.F, 200.F, 0.F));
  rect->SetSize(glm::vec2(400, 400));

  const auto childRect = rect->AddChild(new Rectangle());
  childRect->SetSize(glm::vec2(100, 100));
  rect->Update();

  EXPECT_VEC_EQ(rect->GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0))
  EXPECT_VEC_EQ(rect->GetScissorRect().Size, glm::ivec2(400, 400))
  EXPECT_VEC_EQ(childRect->GetScissorRect().LowerLeftPosition,
                glm::ivec2(150, 150))
  EXPECT_VEC_EQ(childRect->GetScissorRect().Size, glm::ivec2(100, 100))

  rect->SetPadding(glm::vec4(50));
  rect->Update();
  EXPECT_VEC_EQ(childRect->GetScissorRect().LowerLeftPosition,
                glm::ivec2(150, 150))
  EXPECT_VEC_EQ(childRect->GetScissorRect().Size, glm::ivec2(100, 100))

  rect->SetPadding(glm::vec4(200, 200, 0, 0));
  rect->Update();
  EXPECT_VEC_EQ(childRect->GetScissorRect().LowerLeftPosition,
                glm::ivec2(200, 150))
  EXPECT_VEC_EQ(childRect->GetScissorRect().Size, glm::ivec2(50, 50))

  rect->SetPadding(glm::vec4(0, 0, 200, 200));
  rect->Update();
  EXPECT_VEC_EQ(childRect->GetScissorRect().LowerLeftPosition,
                glm::ivec2(150, 200))
  EXPECT_VEC_EQ(childRect->GetScissorRect().Size, glm::ivec2(50, 50))
}

TEST_F(RectangleTest, UpdateVisibility) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());

  root.Update();
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
  EXPECT_VEC_EQ(rect->GetSize(), glm::ivec2(400, 300))

  root.SetPadding(glm::ivec4(50));
  root.Update();
  EXPECT_VEC_EQ(rect->GetSize(), glm::ivec2(350, 250));
}

TEST_F(RectangleTest, UpdateSizeWithAspectRatio) {
  auto rect = Rectangle();

  rect.SetRelativeSize(glm::vec2(1.F, 0.F));
  rect.SetAspectRatio(2.F / 1.F);
  rect.UpdateSize(glm::ivec2(100, 100));
  EXPECT_VEC_EQ(rect.GetSize(), glm::ivec2(100, 50))

  rect.SetRelativeSize(glm::vec2(0.F, 1.F));
  rect.SetAspectRatio(2.F / 1.F);
  rect.UpdateSize(glm::ivec2(100, 100));
  EXPECT_VEC_EQ(rect.GetSize(), glm::ivec2(200, 100))
}

class DerivedRect : public Rectangle {};
TEST_F(RectangleTest, GuiRoot) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());
  const auto childRect = rect->AddChild(new Rectangle());
  const auto nonRootedRect = Rectangle();
  const auto derivedRect = root.AddChild(new DerivedRect());

  EXPECT_EQ(rect->GuiRoot(), &root);
  EXPECT_EQ(childRect->GuiRoot(), &root);
  EXPECT_EQ(derivedRect->GuiRoot(), &root);
  EXPECT_EQ(nonRootedRect.GuiRoot(), nullptr);
}

TEST_F(RectangleTest, ApplyAnchor) {
  auto root = Root(glm::ivec2(400, 400));
  const auto rect = root.AddChild(new Rectangle());
  rect->SetSize(glm::vec2(10, 10));
  rect->SetAnchor(Rectangle::HorizontalAnchors::Center,
                  Rectangle::VerticalAnchors::Middle);
  rect->Update();
  EXPECT_VEC_EQ(rect->GetPosition(), glm::vec3(200.F, 200.F, LAYER_Z_BOTTOM))

  rect->SetAnchor(Rectangle::HorizontalAnchors::Left,
                  Rectangle::VerticalAnchors::Top);
  rect->Update();
  EXPECT_VEC_EQ(rect->GetPosition(), glm::vec3(5.F, 395.F, LAYER_Z_BOTTOM))

  rect->SetAnchor(Rectangle::HorizontalAnchors::Right,
                  Rectangle::VerticalAnchors::Bottom);
  rect->Update();
  EXPECT_VEC_EQ(rect->GetPosition(), glm::vec3(395.F, 5.F, LAYER_Z_BOTTOM))
}

TEST_F(RectangleTest, ApplyAnchorWithPadding) {
  auto root = Root(glm::ivec2(400, 400));
  root.SetPadding(glm::vec4(10.F, 5.F, 15.F, 2.F));
  const auto rect = root.AddChild(new Rectangle());
  rect->SetSize(glm::vec2(10, 10));
  rect->SetAnchor(Rectangle::HorizontalAnchors::Center,
                  Rectangle::VerticalAnchors::Middle);
  rect->Update();
  EXPECT_VEC_EQ(rect->GetPosition(), glm::vec3(195.F, 197.F, LAYER_Z_BOTTOM))

  rect->SetAnchor(Rectangle::HorizontalAnchors::Left,
                  Rectangle::VerticalAnchors::Top);
  rect->Update();
  EXPECT_VEC_EQ(rect->GetPosition(), glm::vec3(15.F, 390.F, LAYER_Z_BOTTOM))

  rect->SetAnchor(Rectangle::HorizontalAnchors::Right,
                  Rectangle::VerticalAnchors::Bottom);
  rect->Update();
  EXPECT_VEC_EQ(rect->GetPosition(), glm::vec3(380.F, 7.F, LAYER_Z_BOTTOM))
}

}  // namespace soil::stage::scene::gui
