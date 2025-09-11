#include "stage/stage.h"

#include "gtest/gtest.h"

namespace soil::stage {
    class StageTest : public testing::Test {};

    TEST_F(StageTest, Contruct) {
        const auto stage = Stage();

        EXPECT_EQ(stage.IsLoaded(), false);
        EXPECT_THROW(stage.GetResources(), StageNotRegisteredException);
    }
} // namespace soil::stage
