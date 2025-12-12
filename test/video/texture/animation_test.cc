#include "video/texture/animation.h"

#include <gtest/gtest.h>

namespace soil::video::texture {
class AnimationTest : public testing::Test {
 protected:
  static std::vector<Animation::Frame> MakeFrames() {
    return {{
                .Index = 0,
                .Duration = 100,
            },
            {
                .Index = 1,
                .Duration = 100,
            }};
  }
};

TEST_F(AnimationTest, Contructor) {
  const auto ticksPerSecond = 25;
  const auto tickDuration = 1.F / static_cast<float>(ticksPerSecond) * 1000.F;
  const auto frames = MakeFrames();
  const auto animation = Animation(&frames, ticksPerSecond);

  EXPECT_EQ(animation.Frames(), &frames);
  EXPECT_EQ(animation.CurrentFrame(), frames[0]);
  EXPECT_EQ(animation.TickDuration(), tickDuration);
}

TEST_F(AnimationTest, Update) {
  const auto frames = MakeFrames();
  auto animation = Animation(&frames, 10);  // 1 tick = 100ms duration
  animation.Update();
  EXPECT_EQ(animation.CurrentFrame(), frames[0]);

  animation.Update();
  EXPECT_EQ(animation.CurrentFrame(), frames[1]);

  animation.Update();
  EXPECT_EQ(animation.CurrentFrame(), frames[1]);

  animation.Update();
  EXPECT_EQ(animation.CurrentFrame(), frames[0]);
}

TEST_F(AnimationTest, SetCurrentFrameIndex) {
  const auto frames = MakeFrames();
  auto animation = Animation(&frames, 25);

  animation.SetFrameIndex(1);
  EXPECT_EQ(animation.CurrentFrame(), frames[1]);

  animation.SetFrameIndex(2);  // invalid
  EXPECT_EQ(animation.CurrentFrame(), frames[1]);

  animation.SetFrameIndex(0);
  EXPECT_EQ(animation.CurrentFrame(), frames[0]);

  animation.SetFrameIndex(-2);  // invalid
  EXPECT_EQ(animation.CurrentFrame(), frames[0]);
}

TEST_F(AnimationTest, SetFrames) {
  const auto frames = MakeFrames();
  auto animation = Animation(nullptr, 25);

  EXPECT_EQ(animation.CurrentFrame(), Animation::EMPTY_FRAME);

  animation.SetFrames(&frames);
  EXPECT_EQ(animation.CurrentFrame(), frames[0]);

  animation.SetFrames(nullptr);
  EXPECT_EQ(animation.CurrentFrame(), Animation::EMPTY_FRAME);
}
}  // namespace soil::video::texture
