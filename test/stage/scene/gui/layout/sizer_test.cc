#include "stage/scene/gui/layout/sizer.h"

#include <gmock/gmock-matchers.h>

#include "gtest/gtest.h"
#include "testing.h"
#include "util/strings.h"

namespace soil::stage::scene::gui::layout {
class SizerTest : public testing::Test {};

TEST_F(SizerTest, CalculateRelativeSize) {
  auto actual = Sizer::CalculateRelativeSize();
}

}  // namespace soil::stage::scene::gui::layout
