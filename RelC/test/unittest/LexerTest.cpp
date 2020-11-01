#include "gtest/gtest.h"
#include "RelC/src/Lexer.h"

class LexerTestFixture : public ::testing::Test, public Lexer
{
protected:
  LexerTestFixture() : Lexer(logger), lexer_test(logger)
  {

  }

  void SetUp() override {
      token_list = new std::vector<Token>();
      //logger.SetLogLevel(LogLevel::DEBUG);
  }

  void TearDown() override
  {
      delete token_list;
  }

  Logger logger;
  std::string testdata;
  Lexer lexer_test;
};

TEST_F(LexerTestFixture, TypeDefinition)
{
    testdata = "type XXX { attribute : id, status : New, }";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);
    lexer_test.Read(d);

    EXPECT_EQ(d.token_list.size(), 12);
}

TEST_F(LexerTestFixture, EnumDefinition)
{
    testdata = "enum XXX { MyAttr }";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);
    lexer_test.Read(d);

    EXPECT_EQ(d.token_list.size(), 5);
}

TEST_F(LexerTestFixture, LineComment)
{
    testdata = "// test line comment\n";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);
    lexer_test.Read(d);

    EXPECT_EQ(d.token_list.size(), 5);
}

TEST_F(LexerTestFixture, IsIdentifier)
{
    EXPECT_TRUE ( IsIdentifier("asbd") );
    EXPECT_TRUE ( IsIdentifier("Xhdsa") );
    EXPECT_TRUE ( IsIdentifier("JDASU") );
    EXPECT_TRUE ( IsIdentifier("ash_dhuzd") );
    EXPECT_TRUE ( IsIdentifier("XB_dhs_ASdzz") );

    EXPECT_FALSE ( IsIdentifier("1abs") );
    EXPECT_FALSE ( IsIdentifier("/§&dasdas") );
    EXPECT_FALSE ( IsIdentifier("asdhdja_&dhas") );
    EXPECT_FALSE ( IsIdentifier("-1273t67") );
    EXPECT_FALSE ( IsIdentifier("1237") );
    EXPECT_FALSE ( IsIdentifier(" ") );
}

TEST_F(LexerTestFixture, IsString)
{
    EXPECT_TRUE ( IsString("asbd") );
    EXPECT_TRUE ( IsString("A") );

    EXPECT_TRUE ( IsString("Xh---dsa") );
    EXPECT_FALSE ( IsString("Xh-\r-dsa") );
    EXPECT_FALSE ( IsString("Xh-\n-dsa") );
    EXPECT_FALSE ( IsString("Xh-\r\n-dsa") );
    EXPECT_FALSE ( IsString("\r") );
    EXPECT_FALSE ( IsString("\n") );
    EXPECT_FALSE ( IsString(" ") );
    EXPECT_FALSE ( IsString("     xx  ") );
}

TEST_F(LexerTestFixture, IsInteger) {
    EXPECT_TRUE ( IsInteger("-1") );
    EXPECT_TRUE ( IsInteger("0") );
    EXPECT_TRUE ( IsInteger("1") );
    EXPECT_TRUE ( IsInteger("17262") );
    EXPECT_TRUE ( IsInteger("-14726") );

    EXPECT_FALSE ( IsInteger("-1x4726") );
    EXPECT_FALSE ( IsInteger("1x4726") );
    EXPECT_FALSE ( IsInteger("$1x4726") );
    EXPECT_FALSE ( IsInteger("a-1x4726") );
    EXPECT_FALSE ( IsInteger("X") );
    EXPECT_FALSE ( IsInteger("123sa") );
}

TEST_F(LexerTestFixture, IsDelimiter) {
    EXPECT_TRUE ( IsDelimiter('\n') );
    EXPECT_TRUE ( IsDelimiter('\r') );
    EXPECT_TRUE ( IsDelimiter(' ') );

    EXPECT_FALSE ( IsDelimiter('a') );
    EXPECT_FALSE ( IsDelimiter('x') );
    EXPECT_FALSE ( IsDelimiter('_') );
    EXPECT_FALSE ( IsDelimiter(0) );
}

TEST_F(LexerTestFixture, IsLinebreak) {
    EXPECT_TRUE ( IsLinebreak("\r\n") );
    EXPECT_TRUE ( IsLinebreak("\n") );

    EXPECT_FALSE ( IsLinebreak("ads") );
    EXPECT_FALSE ( IsLinebreak("") );
    EXPECT_FALSE ( IsLinebreak("_") );
}

TEST_F(LexerTestFixture, IsWhitespace) {
    EXPECT_TRUE ( IsWhitespace(' ') );
    EXPECT_TRUE ( IsWhitespace('\t') );

    EXPECT_FALSE ( IsWhitespace('x') );
    EXPECT_FALSE ( IsWhitespace(0) );
    EXPECT_FALSE ( IsWhitespace('_') );
}

TEST_F(LexerTestFixture, IdentifyTokenInString1) {
    std::string xx = "string";
    CheckStringandAddToken(xx, ',');

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::STRING);
}

TEST_F(LexerTestFixture, IdentifyTokenInString2) {
    std::string xx = "stringXX";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::IDENTIFIER);
}

TEST_F(LexerTestFixture, IdentifyTokenInString3) {
    std::string xx = "hello";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::IDENTIFIER);
}

TEST_F(LexerTestFixture, IdentifyTokenInString4) {
    std::string xx = "43847";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::INTEGER_VALUE);
}

TEST_F(LexerTestFixture, IdentifyTokenInString5) {
    std::string xx = "438_47";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::STRING_VALUE);
}

TEST_F(LexerTestFixture, IdentifyTokenInString6) {
    std::string xx = ":";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::COLON);
}

TEST_F(LexerTestFixture, IdentifyTokenInString7) {
    std::string xx = "//";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::LINE_COMMENT);
}

TEST_F(LexerTestFixture, IdentifyTokenInString8) {
    std::string xx = "/*";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::COMMENT_BLOCK_START);
}

TEST_F(LexerTestFixture, IdentifyTokenInString9) {
    std::string xx = "*/";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::COMMENT_BLOCK_END);
}

TEST_F(LexerTestFixture, IdentifyTokenInString10) {
    std::string xx = ",";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::COMMA);
}

TEST_F(LexerTestFixture, IdentifyTokenInString11) {
    std::string xx = "{";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::BRACKET_OPEN);
}

TEST_F(LexerTestFixture, IdentifyTokenInString12) {
    std::string xx = "link";
    CheckStringandAddToken(xx, ',');

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::LINK);
}

TEST_F(LexerTestFixture, IdentifyTokenInString13) {
    std::string xx = "linkXX";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::IDENTIFIER);
}

TEST_F(LexerTestFixture, IdentifyTokenInString14) {
    std::string xx = "}";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::BRACKET_CLOSE);
}

TEST_F(LexerTestFixture, IdentifyTokenInString15) {
    std::string xx = "id";
    CheckStringandAddToken(xx, ',');

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::ID);
}

TEST_F(LexerTestFixture, IdentifyTokenInString16) {
    std::string xx = "idXX";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::IDENTIFIER);
}

TEST_F(LexerTestFixture, IdentifyTokenInString17) {
    std::string xx = "type";
    CheckStringandAddToken(xx, '{');

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::TYPE);
}

TEST_F(LexerTestFixture, IdentifyTokenInString18) {
    std::string xx = "typeXX";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::IDENTIFIER);
}

TEST_F(LexerTestFixture, IdentifyTokenInString19) {
    std::string xx = "enum";
    CheckStringandAddToken(xx, '{');

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::ENUM);
}

TEST_F(LexerTestFixture, IdentifyTokenInString20) {
    std::string xx = "enumXX";
    CheckStringandAddToken(xx);

    EXPECT_EQ(token_list->size(), 1);
    EXPECT_EQ((*token_list)[0].GetTokenType(), TokenType::IDENTIFIER);
}