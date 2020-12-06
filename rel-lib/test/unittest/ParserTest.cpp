#include "gtest/gtest.h"
#include "rel-lib/src/Parser.h"

class ParserTestFixture : public ::testing::Test, public Parser
{
protected:
  ParserTestFixture() : Parser(logger)
  {

  }

  void SetUp() override {
      testdata_rd = new FileTokenData(DataType::RequirementsData);
      testdata_rs = new FileTokenData(DataType::RequirementsSpecification);
      eol = new Token("\n", TokenType::END_OF_LINE, "", 0, 0);
      random_string = new Token("dhasjhdkas_dasdsa", TokenType::STRING_VALUE, "", 0, 0);
      index = 0;
  }

  void TearDown() override
  {
      delete testdata_rs;
      delete testdata_rd;
      delete eol;
  }

  unsigned int index;
  Logger logger;
  FileTokenData *testdata_rs;
  FileTokenData *testdata_rd;

  Token *eol;
  Token *random_string;
};

TEST_F(ParserTestFixture, IsNextTokenCorrectlyIdentified)
{
    for(int i=0; i<3; i++)
        testdata_rs->token_list.push_back(*eol);

    Token colon("", TokenType::COLON, "", 0, 0);
    testdata_rs->token_list.push_back(colon);

    EXPECT_TRUE(IsNextToken(*testdata_rs, index, TokenType::COLON));
    index = 0;
    EXPECT_FALSE(IsNextToken(*testdata_rs, index, TokenType::ID));
}

TEST_F(ParserTestFixture, IsNextTokenCorrectlyIdentified2)
{
    Token enumx("//", TokenType::ENUM, "", 0, 0);
    testdata_rs->token_list.push_back(enumx);
    EXPECT_TRUE(IsNextToken(*testdata_rs, index, TokenType::ENUM));
    index = 0;
    EXPECT_FALSE(IsNextToken(*testdata_rs, index, TokenType::BRACKET_CLOSE));
}

TEST_F(ParserTestFixture, MultiLineCommentRead)
{
   int tokens_added = 0;
   Token comment_start("/*", TokenType::COMMENT_BLOCK_START, "", 0, 0);
   testdata_rs->token_list.push_back(comment_start);
   tokens_added++;
   for(; tokens_added<15; tokens_added++)
   {
      if(tokens_added%2 == 0)
        testdata_rs->token_list.push_back(*random_string);
      else
      {
        testdata_rs->token_list.push_back(*eol);
      }
    }

    Token comment_end("*/", TokenType::COMMENT_BLOCK_END, "", 0, 0);
    testdata_rs->token_list.push_back(comment_end);
    tokens_added++;

    for(int i=0;i<5; i++)
      testdata_rs->token_list.push_back(*random_string);

    MultiLineComment(*testdata_rs, index);

    EXPECT_EQ(tokens_added-1, index);
}

TEST_F(ParserTestFixture, ReadIdentifier)
{
    Token ident("MyIdentifier", TokenType::IDENTIFIER, "", 0, 0);
    testdata_rs->token_list.push_back(ident);

    RsRdIdentifier r = Identifier(*testdata_rs, index);

    EXPECT_EQ(r.name, ident.GetTokenValue());
}

TEST_F(ParserTestFixture, LineCommentRead)
{
   int tokens_added = 0;
   Token linecomment("", TokenType::LINE_COMMENT, "", 0, 0);
   testdata_rs->token_list.push_back(linecomment);
   tokens_added++;
   for(; tokens_added<5; tokens_added++)
      testdata_rs->token_list.push_back(*random_string);

    testdata_rs->token_list.push_back(*eol);
    tokens_added++;
    for(int i=0;i<5; i++)
      testdata_rs->token_list.push_back(*random_string);

    LineComment(*testdata_rs, index);

    EXPECT_EQ(tokens_added-1, index);
}