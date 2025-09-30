#include "stage/scene/gui/rectangle.h"

#include <utility>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "stage/scene/gui/root.h"
#include "testing.h"
#include "util/strings.h"

namespace soil::stage::scene::gui {
class RectangleTest : public testing::Test {};

TEST_F(RectangleTest, Contruct) {
  const auto rect = Rectangle();

  EXPECT_VEC_EQ(rect.GetPosition(),
                glm::vec3(0.F, 0.F, Rectangle::LAYER_Z_INCREMENT));
  EXPECT_VEC_EQ(rect.GetRelativeSize(), glm::vec2(0.F));
  EXPECT_VEC_EQ(rect.GetAspectRatio(), glm::vec2(0.F));
  EXPECT_VEC_EQ(rect.GetChildSize(), rect.GetSize());
}

TEST_F(RectangleTest, UpdateScissor) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());
  rect->SetSize(glm::vec2(400, 400));
  rect->SetPosition(
      glm::vec3(-400.F, -300.F, 0.F));  // center(0, 0) -> only 200px visible

  rect->Update();
  EXPECT_VEC_EQ(rect->GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0))
  EXPECT_VEC_EQ(rect->GetScissorRect().Size, glm::ivec2(200, 200))

  rect->SetPosition(glm::vec3(
      -400.F, -100.F, 0.F));  // center(0, 200) -> 400px of height visible
  rect->Update();
  EXPECT_VEC_EQ(rect->GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0))
  EXPECT_VEC_EQ(rect->GetScissorRect().Size, glm::ivec2(200, 400))

  rect->SetPosition(
      glm::vec3(-200.F, -100.F, 0.F));  // center(200, 200) -> 400px visible
  rect->Update();
  EXPECT_VEC_EQ(rect->GetScissorRect().LowerLeftPosition, glm::ivec2(0, 0))
  EXPECT_VEC_EQ(rect->GetScissorRect().Size, glm::ivec2(400, 400))

  rect->SetPosition(
      glm::vec3(100.F, 200.F,
                0.F));  // center(400, 400) -> 400px width, 350px height visible
  rect->Update();
  EXPECT_VEC_EQ(rect->GetScissorRect().LowerLeftPosition, glm::ivec2(300, 300))
  EXPECT_VEC_EQ(rect->GetScissorRect().Size, glm::ivec2(400, 300))
}

TEST_F(RectangleTest, UpdateScissorWithPadding) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());
  rect->SetPosition(glm::vec3(-200.F, -100.F, 0.F));
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

TEST_F(RectangleTest, UpdateSize) {
  auto root = Root(glm::ivec2(800, 600));
  const auto rect = root.AddChild(new Rectangle());

  rect->SetRelativeSize(glm::vec2(0.5F, 0.5F));
  root.Update();
  EXPECT_VEC_EQ(rect->GetSize(), glm::ivec2(400, 300))

  root.SetPadding(glm::ivec4(50));
  root.Update();
  EXPECT_VEC_EQ(rect->GetSize(), glm::ivec2(350, 250));
}

}  // namespace soil::stage::scene::gui
