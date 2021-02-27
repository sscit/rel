#include "gtest/gtest.h"
#include "rel-lib/src/Token.h"

class TokenFixture : public ::testing::Test, public Token {
protected:
  TokenFixture() : Token() {

  }

  void SetUp() override {
      //logger.SetLogLevel(LogLevel::DBUG);
  }

  void TearDown() override {

  }

  std::string token_value = "id";
  std::string filename = "data.rd";
};

TEST_F(TokenFixture, CheckGetMethods) {
    Token t(token_value, TokenType::ID, filename, 123, 23);

    EXPECT_EQ(t.GetFilename()->compare(filename), 0);
    EXPECT_EQ(t.GetLineNumberOfToken(), 123);
    EXPECT_EQ(t.GetPositionInLineOfToken(), 23);
    EXPECT_EQ(t.GetTokenType(), TokenType::ID);
    EXPECT_EQ(t.GetTokenValue()->compare(token_value), 0);
}

TEST_F(TokenFixture, CompareTokenValue) {
    Token t(token_value, TokenType::ID, filename, 123, 23);

    EXPECT_TRUE(t.Compare("id"));
    EXPECT_FALSE(t.Compare("id2"));
}

