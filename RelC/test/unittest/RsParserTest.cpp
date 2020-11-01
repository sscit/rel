#include "gtest/gtest.h"
#include "RelC/src/RsParser.h"

class RsParserTestFixture : public ::testing::Test, public RsParser
{
protected:
  RsParserTestFixture() : RsParser(logger), lexer_test(logger)
  {

  }

  void SetUp() override {
      
  }

  void TearDown() override
  {
      
  }

  Logger logger;
  std::string testdata;
  Lexer lexer_test;
};

TEST_F(RsParserTestFixture, TypeDefinitionWithComments)
{
    /* Note that the backslash "\"" is already stripped away by the
       C++ parser. It is used to break this string into multiple lines in C++ editor,
       to make the test more readable. It is not part of the testdata string, which is parsed.
    */
    testdata = "type XXX {\n               \
                // bla blab lab la\n       \
                attribute : id,\n          \
                /* bla blab lab la\n       \
                 bla blab lab la  \n       \
                 bla blab lab la  \n       \
                 bla blab lab la  \n       \
                 bla blab lab la  \n       \
                */\n                       \
                attribtue2 : link,\n       \
                // bla blab lab la\n       \
                attr3 : string,}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ParseTokens(d);

    EXPECT_EQ(all_enums.size(), 0);
    EXPECT_EQ(all_types.size(), 1);
    EXPECT_EQ(all_types["XXX"].type_elements.size(), 3);
}

TEST_F(RsParserTestFixture, TypeDefinition)
{
    testdata = "type XXX { attribute : id, attribtue2 : link, attr3 : string, attr4 : int,}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ParseTokens(d);

    EXPECT_EQ(all_enums.size(), 0);
    EXPECT_EQ(all_types.size(), 1);
    EXPECT_EQ(all_types["XXX"].type_elements.size(), 4);
}

TEST_F(RsParserTestFixture, WrongToken)
{
    testdata = ",type XXX { attribute  id, attribtue2 : link, attr3 : string, attr4 : int,}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ASSERT_THROW(ParseTokens(d), WrongTokenException);
}

TEST_F(RsParserTestFixture, WrongTypeDefinition)
{
    testdata = "type XXX { attribute  id, attribtue2 : link, attr3 : string, attr4 : int,}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ASSERT_THROW(ParseTokens(d), RsTypeException);
}

TEST_F(RsParserTestFixture, WrongTypeDefinition2)
{
    testdata = "type XXX { attribute : id, attribtue2 : link, attr3 : string, attr4 : int}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ASSERT_THROW(ParseTokens(d), RsTypeException);
}

TEST_F(RsParserTestFixture, TypeAndEnum)
{
    testdata = "enum XXX { A,B,C,} type MyType { att1 : id, /* blabal l */ att2 : XXX, att3 : int,}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ParseTokens(d);
    
    EXPECT_EQ(all_enums.size(), 1);
    EXPECT_EQ(all_enums["XXX"].enum_elements.size(), 3);
    EXPECT_EQ(all_types.size(), 1);
    EXPECT_EQ(all_types["MyType"].type_elements.size(), 3);
}

TEST_F(RsParserTestFixture, EnumDefinitions)
{
    testdata = "enum XXX { A,B,C,} enum YYY { a,b,c,d,e,}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ParseTokens(d);
    
    EXPECT_EQ(all_enums.size(), 2);
    EXPECT_EQ(all_enums["XXX"].enum_elements.size(), 3);
    EXPECT_EQ(all_types.size(), 0);
}

TEST_F(RsParserTestFixture, WrongEnumDefinition)
{
    testdata = "enum XXX { ,A,B,C,}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ASSERT_THROW(ParseTokens(d), RsEnumException);
}

TEST_F(RsParserTestFixture, WrongEnumDefinition2)
{
    testdata = "enumX XXX { A,B,C,}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ASSERT_THROW(ParseTokens(d), WrongTokenException);
}

TEST_F(RsParserTestFixture, WrongEnumDefinition3)
{
    testdata = "enum XXX { A,B,B,}";

    FileReader r(testdata);
    FileTokenData d(DataType::RequirementsSpecification, r);

    lexer_test.Read(d);
    ASSERT_THROW(ParseTokens(d), RsEnumException);
}
