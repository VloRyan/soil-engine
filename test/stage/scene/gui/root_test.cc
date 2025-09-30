#include "stage/scene/gui/root.h"

#include <utility>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "stage/scene/gui/rectangle.h"
#include "testing.h"

namespace soil::stage::scene::gui {
class RootTest : public testing::Test {};

TEST_F(RootTest, Contructor) {
  const auto root = Root(glm::ivec2(800, 600));

  EXPECT_EQ(root.GetSize(), glm::ivec2(800, 600));
}

TEST_F(RootTest, MouseOver) {
  auto root = Root(glm::ivec2(800, 600));
  auto* item1 = root.AddChild(new Rectangle());
  item1->SetPosition(glm::vec3(-300, 200, 0));
  auto* item2 = root.AddChild(new Rectangle());
  item2->SetPosition(glm::vec3(300, 200, 0));
  auto* item3 = root.AddChild(new Rectangle());
  item3->SetPosition(glm::vec3(-300, -200, 0));
  auto* subItem = item3->AddChild(new Rectangle());
  root.Update();

  root.Handle(input::Event::MakeMousePositionEvent(glm::vec2(100.F, 100.F)));
  EXPECT_TRUE(item1->IsMouseOver());
  EXPECT_FALSE(item2->IsMouseOver());
  EXPECT_FALSE(item3->IsMouseOver());
  EXPECT_FALSE(subItem->IsMouseOver());

  root.Handle(input::Event::MakeMousePositionEvent(glm::vec2(700.F, 100.F)));
  EXPECT_FALSE(item1->IsMouseOver());
  EXPECT_TRUE(item2->IsMouseOver());
  EXPECT_FALSE(item3->IsMouseOver());
  EXPECT_FALSE(subItem->IsMouseOver());

  root.Handle(input::Event::MakeMousePositionEvent(glm::vec2(100.F, 500.F)));
  EXPECT_FALSE(item1->IsMouseOver());
  EXPECT_FALSE(item2->IsMouseOver());
  EXPECT_TRUE(item3->IsMouseOver());
  EXPECT_TRUE(subItem->IsMouseOver());
}
}  // namespace soil::stage::scene::gui
