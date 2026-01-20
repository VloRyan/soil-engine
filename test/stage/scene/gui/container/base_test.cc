#include "stage/scene/gui/container/base.h"

#include "gtest/gtest.h"
#include "testing.h"

namespace soil::stage::scene::gui::container {
class BaseTest : public testing::Test {
 protected:
  class BaseSpy : public Base {
   public:
    BaseSpy() : Base() {}
    BaseSpy(glm::ivec2 size, glm::ivec2 rectSize) : Base() {
      size_ = size;
      scissorRect_.Size = rectSize;
    }
    void OnMouseWheel(const glm::ivec2& pos, glm::vec2 offset) override {
      Base::OnMouseWheel(pos, offset);
    }
  };
};

TEST_F(BaseTest, Contructor) {
  const auto base = BaseSpy();

  EXPECT_EQ(base.GetScrollStep(), glm::ivec2(10));
  EXPECT_EQ(base.GetOffset(), glm::ivec2(0));
  EXPECT_EQ(base.GetMargin(), 0);
}

TEST_F(BaseTest, OnMouseWheel) {
  auto base = BaseSpy(glm::ivec2(220), glm::ivec2(200));

  ASSERT_EQ(base.GetOffset(), glm::ivec2(0));
  ASSERT_EQ(base.GetScrollStep(), glm::ivec2(10));

  base.OnMouseWheel(glm::ivec2(0), glm::ivec2(0));
  EXPECT_VEC_EQ(base.GetOffset(), glm::ivec2(0));

  base.OnMouseWheel(glm::ivec2(0), glm::ivec2(-5, 0));
  EXPECT_VEC_EQ(base.GetOffset(), glm::ivec2(0, 0));

  base.OnMouseWheel(glm::ivec2(0), glm::ivec2(6, 0));
  EXPECT_VEC_EQ(base.GetOffset(), glm::ivec2(10, 0));

  base.OnMouseWheel(glm::ivec2(0), glm::ivec2(-6, 0));
  EXPECT_VEC_EQ(base.GetOffset(), glm::ivec2(0, 0));

  base.OnMouseWheel(glm::ivec2(0), glm::ivec2(0, -8));
  EXPECT_VEC_EQ(base.GetOffset(), glm::ivec2(0, 10));

  base.OnMouseWheel(glm::ivec2(0), glm::ivec2(0, 3));
  EXPECT_VEC_EQ(base.GetOffset(), glm::ivec2(0, 0));

  base.OnMouseWheel(glm::ivec2(0), glm::ivec2(0, 10));
  EXPECT_VEC_EQ(base.GetOffset(), glm::ivec2(0, 0));

  base.SetScrollStep(glm::ivec2(25));
  base.OnMouseWheel(glm::ivec2(0), glm::ivec2(1, -1));
  EXPECT_VEC_EQ(base.GetOffset(), glm::ivec2(20, 20));

}
}  // namespace soil::stage::scene::gui::container
