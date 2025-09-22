#include "stage/scene/component/bounding_volume.h"

#include <gtest/gtest.h>

namespace soil::stage::scene::component {
    class BoundingVolumeTest : public testing::Test {};

    TEST_F(BoundingVolumeTest, Contruct) {
        const auto volume = BoundingVolume(nullptr);
    }
} // namespace soil::stage::scene::component
