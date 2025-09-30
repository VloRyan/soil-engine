#include "file/font.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <resources.h>

#include "file/sprite_sheet.h"

namespace soil::file {
class FontTest : public testing::Test {};

TEST_F(FontTest, Load) {
  const auto filePath = resources::GetPath("file/Font.fnt");
  const std::vector<Font::Character> characters = {
      {
          .Id = 'a',
          .TextureCoord = {1, 2},
          .Offset = {1, 2},
          .Size = {1, 2},
          .XAdvance = 1,
      },
      {
          .Id = 'b',
          .TextureCoord = {-1, -2},
          .Offset = {-1, -2},
          .Size = {-1, -2},
          .XAdvance = -1,
      },
  };
  const std::vector<Font::Kerning> kernings = {
      {
          .First = 'a',
          .Second = 'b',
          .Amount = 1,
      },
      {
          .First = 'b',
          .Second = 'a',
          .Amount = -1,
      },
  };
  auto* file = Font::Load(filePath);

  EXPECT_EQ(file->FileName, filePath);
  EXPECT_EQ(file->Padding, glm::ivec4(8));
  EXPECT_EQ(file->ImageSize, glm::ivec2(256, 512));
  EXPECT_EQ(file->TextureFileName, resources::GetPath("file/Test.png"));
  EXPECT_EQ(file->Base, 59);
  EXPECT_EQ(file->LineHeight, 100);

  EXPECT_EQ(file->Characters['a'], characters[0]);
  EXPECT_EQ(file->Characters['b'], characters[1]);
  EXPECT_THAT(file->Kernings, testing::ElementsAre(kernings[0], kernings[1]));
}
}  // namespace soil::file
