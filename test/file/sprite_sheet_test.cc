#include "file/sprite_sheet.h"

#include <gtest/gtest.h>

namespace soil::file {
    class SpriteSheetTest : public testing::Test {};

    TEST_F(SpriteSheetTest, SaveLoad) {
        const std::filesystem::path tempFilePath = std::filesystem::temp_directory_path() / std::tmpnam(nullptr);
        auto expected = SpriteSheet {.TextureFileName = "texture/texture.png",
                                     .TilesPerDim = 2,
                                     .Tiles = {{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}},
                                     .Sequences = {{"all", {.FrameIndices = {1, 2, 3, 4}, .TicksPerFrame = 8}}}};


        expected.Save(tempFilePath);
        auto actual = SpriteSheet::Load(tempFilePath);

        EXPECT_EQ(actual, expected);

        std::filesystem::remove(tempFilePath);
    }


} // namespace soil::file
