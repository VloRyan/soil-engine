#include "stage/scene/component/text/parser.h"

#include <gtest/gtest.h>

#include "resources.h"

namespace soil::stage::scene::component::text {
class ParserTest : public testing::Test {
 protected:
  static Glyph CharacterGlyph(const file::Font& font, char c) {
    return Glyph{
        .Type = Glyph::Type::Character,
        .Character = &font.Characters.at(c),
    };
  }
};

TEST_F(ParserTest, ParseWords) {
  auto text = "one two three";
  auto font = file::Font::Load(resources::GetPath("file/Symmetric.fnt"));
  auto lines = Parser::Parse(text, font->Characters);

  EXPECT_EQ(lines.size(), 1);
  EXPECT_EQ(lines[0].Text, text);
  EXPECT_EQ(lines[0].Length, 828);  // 12 * 64 + 1 * 60
  EXPECT_EQ(lines[0].Words.size(), 3);
  EXPECT_EQ(lines[0].Words[0].Text, "one ");
  EXPECT_EQ(lines[0].Words[0].Length, 256);
  EXPECT_EQ(lines[0].Words[0].Glyphs.size(), 4);
  EXPECT_EQ(lines[0].Words[0].Glyphs[0], CharacterGlyph(*font, 'o'));
  EXPECT_EQ(lines[0].Words[0].Glyphs[1], CharacterGlyph(*font, 'n'));
  EXPECT_EQ(lines[0].Words[0].Glyphs[2], CharacterGlyph(*font, 'e'));
  EXPECT_EQ(lines[0].Words[0].Glyphs[3], CharacterGlyph(*font, ' '));

  EXPECT_EQ(lines[0].Words[1].Text, "two ");
  EXPECT_EQ(lines[0].Words[1].Length, 256);
  EXPECT_EQ(lines[0].Words[1].Glyphs.size(), 4);
  EXPECT_EQ(lines[0].Words[1].Glyphs[0], CharacterGlyph(*font, 't'));
  EXPECT_EQ(lines[0].Words[1].Glyphs[1], CharacterGlyph(*font, 'w'));
  EXPECT_EQ(lines[0].Words[1].Glyphs[2], CharacterGlyph(*font, 'o'));
  EXPECT_EQ(lines[0].Words[1].Glyphs[3], CharacterGlyph(*font, ' '));

  EXPECT_EQ(lines[0].Words[2].Text, "three");
  EXPECT_EQ(lines[0].Words[2].Length, 320);
  EXPECT_EQ(lines[0].Words[2].Glyphs.size(), 5);
  EXPECT_EQ(lines[0].Words[2].Glyphs[0], CharacterGlyph(*font, 't'));
  EXPECT_EQ(lines[0].Words[2].Glyphs[1], CharacterGlyph(*font, 'h'));
  EXPECT_EQ(lines[0].Words[2].Glyphs[2], CharacterGlyph(*font, 'r'));
  EXPECT_EQ(lines[0].Words[2].Glyphs[3], CharacterGlyph(*font, 'e'));
  EXPECT_EQ(lines[0].Words[2].Glyphs[4], CharacterGlyph(*font, 'e'));
}

TEST_F(ParserTest, ParseNewLine) {
  auto text = "one\ntwo";
  auto font = file::Font::Load(resources::GetPath("file/Symmetric.fnt"));
  auto lines = Parser::Parse(text, font->Characters);

  EXPECT_EQ(lines.size(), 2);
  EXPECT_EQ(lines[0].Text, "one");
  EXPECT_EQ(lines[0].Length, 188);  // 2 * 64 + 1 * 60

  EXPECT_EQ(lines[1].Text, "two");
  EXPECT_EQ(lines[1].Length, 188);  // 2 * 64 + 1 * 60
}

TEST_F(ParserTest, ParseLineBreak) {
  auto text = "one two";
  auto font = file::Font::Load(resources::GetPath("file/Symmetric.fnt"));
  auto lines = Parser::Parse(text, font->Characters, 190);

  EXPECT_EQ(lines.size(), 2);
  EXPECT_EQ(lines[0].Text, "one ");
  EXPECT_EQ(lines[0].Length, 252);  // 3 * 64 + 1 * 60

  EXPECT_EQ(lines[1].Text, "two");
  EXPECT_EQ(lines[1].Length, 188);  // 2 * 64 + 1 * 60
}

TEST_F(ParserTest, ParseSymbols) {
  auto text = "parsing :known_symbol: :unknown_symbol: also in:word:.";
  auto font = file::Font::Load(resources::GetPath("file/Symmetric.fnt"));
  auto lines =
      Parser::Parse(text, font->Characters,
                    {
                        {"known_symbol",
                         {.Name = "known_symbol", .AdvanceX = 64, .SizeX = 60}},
                        {"word", {.Name = "word", .AdvanceX = 64, .SizeX = 60}},
                    });

  EXPECT_EQ(lines.size(), 1);
  EXPECT_EQ(lines[0].Text, "parsing ¶ :unknown_symbol: also in¶.");
  EXPECT_EQ(
      lines[0].Length,
      2300);  // 35 * 64 + 1 * 60 (:knownSymbol: = 1 glyph, :word: = 1 glyph)
}
}  // namespace soil::stage::scene::component::text
