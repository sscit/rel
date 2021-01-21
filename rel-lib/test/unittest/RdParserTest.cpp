#include "gtest/gtest.h"
#include "rel-lib/src/RdParser.h"

class RdParserTestFixture : public ::testing::Test, public RdParser {
protected:
  RdParserTestFixture() : RdParser(logger, rs_parser), lexer_test(logger), rs_parser(logger) { }

  void SetUp() override { 
    //logger.SetLogLevel(LogLevel::DBUG);
  }

  void TearDown() override { }

  Logger logger;
  std::string spec;
  std::string data;
  Lexer lexer_test;
  RsParser rs_parser;
};

TEST_F(RdParserTestFixture, DatasetWithArrayOfLinks) {
    spec = "type Req { Identifier : id, Text : string, Parent : link,}";
    data = "Req { Identifier : p1, Text : \"Parent Req 1\", Parent : p1,}       \
            Req { Identifier : p2, Text : \"Parent Req 2\", Parent : [p2,],}    \
            Req { Identifier : c1, Text : \"Child Req\", Parent : [p1,p2,],}    ";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ParseTokens(d_data);

    EXPECT_EQ(database.size(), 3);
    EXPECT_EQ(database[0].attributes.size(), 3);
    EXPECT_EQ(database[0].attributes[2].link_value.size(), 1);
    EXPECT_EQ(database[2].attributes[2].link_value.size(), 2);
}

TEST_F(RdParserTestFixture, DatasetWithArrayOfLinks2) {
    spec = "type Req { Identifier : id, Text : string, Parent : link,}";
    data = "Req { Identifier : p1, Text : \"Parent Req 1\", Parent : [p1,p1,p1,p1,p1,p1,],}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ParseTokens(d_data);

    EXPECT_EQ(database.size(), 1);
    EXPECT_EQ(database[0].attributes.size(), 3);
    EXPECT_EQ(database[0].attributes[2].link_value.size(), 6);
}

TEST_F(RdParserTestFixture, EmptyArrayOfLinks) {
    spec = "type Req { Identifier : id, Text : string, Parent : link,}";
    data = "Req { Identifier : p1, Text : \"Parent Req 1\", Parent : [],}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), ArrayException);
}

TEST_F(RdParserTestFixture, ArrayWithWrongLinks) {
    spec = "type Req { Identifier : id, Text : string, Parent : link,}";
    data = "Req { Identifier : p1, Text : \"Parent Req 1\", Parent : [1234],}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), ArrayException);
}

TEST_F(RdParserTestFixture, ArrayWithWrongLinks2) {
    spec = "type Req { Identifier : id, Text : string, Parent : link,}";
    data = "Req { Identifier : p1, Text : \"Parent Req 1\", Parent : [\"id4#udz\"],}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), ArrayException);
}

TEST_F(RdParserTestFixture, ArrayWithSyntaxError) {
    spec = "type Req { Identifier : id, Text : string, Parent : link,}";
    data = "Req { Identifier : p1, Text : \"Parent Req 1\", Parent : [p1],}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), ArrayException);
}

TEST_F(RdParserTestFixture, UsingReservedKeywords) {
    spec = "type Req { unique_id : id, another_id : id, thirdId : id, fid : id , fifthid : id , sixid : id,}";
    data = "Req { unique_id : typeXX, another_id : enumXX, thirdId : idXX, fid : intXX, fifthid : stringXX, sixid : linkXX,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ParseTokens(d_data);

    EXPECT_EQ(database.size(), 1);
    EXPECT_EQ(database[0].attributes.size(), 6);
}

TEST_F(RdParserTestFixture, SingleDataset) {
    spec = "type Req { unique_id : id, Text : string,}";
    data = "Req { unique_id : Req12, Text : \"First Requirement\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ParseTokens(d_data);

    EXPECT_EQ(database.size(), 1);
    EXPECT_EQ(database[0].attributes.size(), 2);
}

TEST_F(RdParserTestFixture, SingleDataset2) {
    spec = "type Req { Identifier : id, Number : int, Text : string, Parent : link,}";
    data = "Req { Identifier : r1, Number : 1234, Text : \"1234\", Parent : r1,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ParseTokens(d_data);

    EXPECT_EQ(database.size(), 1);
    EXPECT_EQ(database[0].attributes.size(), 4);
}

TEST_F(RdParserTestFixture, DatasetWithMaskedQuotationMark) {
    spec = "type Req { Text : string, }";
    data = "Req { Text : \"I would like to \\\"emphasize\\\" this part and mo\\\"re\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ParseTokens(d_data);

    EXPECT_EQ(database.size(), 1);
    EXPECT_EQ(database[0].attributes.size(), 1);
}

TEST_F(RdParserTestFixture, DatasetWithEnum) {
    spec = "type Req { Color : Color,} enum Color {red, blue,}";
    data = "Req { Color : red,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);   
    rs_parser.ParseTokens(d_spec);
    rs_parser.CheckAllEnumTypes();

    RdParser rd_parser(logger, rs_parser);
    rd_parser.ParseTokens(d_data);
}

TEST_F(RdParserTestFixture, ParseErrorWrongToken) {
    spec = "type Req { unique_id : id, Text : string,}";
    data = "Req {{} unique_id : r1, Text : \"First Requirement\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, ParseErrorWrongToken2) {
    spec = "type Req { unique_id : id, Text : string,}";
    data = "type Req { unique_id : r1, Text : \"First Requirement\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), WrongTokenException);
}

TEST_F(RdParserTestFixture, TypeInstanceElementWrong_Order) {
    spec = "type Req { unique_id : id, Text : string,}";
    data = "Req { Text : \"First Requirement\", unique_id : r1,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceElementWrong_WrongAttributeName) {
    spec = "type Req { unique_id : id, Text : string,}";
    data = "Req { unique_id : id1, Text123 : \"First Requirement\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceElementWrong_WrongStringType) {
    spec = "type Req { unique_id : id, Text : string,}";
    data = "Req { unique_id : r1, Text : 123,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceElementWrong_OneElementMissing) {
    spec = "type Req { unique_id : id, Text : string,}";
    data = "Req { unique_id : r1,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceElementWrong_TooManyElements) {
    spec = "type Req { unique_id : id, Text : string,}";
    data = "Req { unique_id : r1, Text : \"mytext\", Text2 : \"one attribute too much\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceWrong_LinkDoesNotExist) {
    spec = "type Req { attr : id, parent : link,}";
    data = "Req { attr : r1, parent : r2,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);
    ParseTokens(d_data);

    ASSERT_THROW(CheckAllLinks(), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceWrong_LinkDoesNotExist2) {
    spec = "type Req { attr : id, parent : link,}";
    data = "Req { attr : r1, parent : [r2,],}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);
    ParseTokens(d_data);

    ASSERT_THROW(CheckAllLinks(), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceWrong_LinkDoesNotExist3) {
    spec = "type Req { attr : id, parent : link,}";
    data = "Req { attr : r1, parent : [r1,r3,],}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);
    ParseTokens(d_data);

    ASSERT_THROW(CheckAllLinks(), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceWrong_WrongLinkValue) {
    spec = "type Req { attr : id, parent : link,}";
    data = "Req { attr : r1, parent : \"r1\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);   
    rs_parser.ParseTokens(d_spec);
        
    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceWrong_WrongStringValue) {
    spec = "type Req { attr : string,}";
    data = "Req { attr : MyString,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);   
    rs_parser.ParseTokens(d_spec);
        
    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceWrong_WrongStringValue2) {
    spec = "type Req { attr : string,}";
    data = "Req { attr : 1234,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);   
    rs_parser.ParseTokens(d_spec);
        
    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceWrong_WrongIntValue) {
    spec = "type Req { attr : int,}";
    data = "Req { attr : \"123\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);   
    rs_parser.ParseTokens(d_spec);
        
    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceWrong_WrongIdValue) {
    spec = "type Req { unique_id : id,}";
    data = "Req { unique_id : \"r1\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);   
    rs_parser.ParseTokens(d_spec);
        
    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, TypeInstanceWrong_WrongIdValue2) {
    spec = "type Req { unique_id : id,}";
    data = "Req { unique_id : 1234,}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);   
    rs_parser.ParseTokens(d_spec);
        
    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

TEST_F(RdParserTestFixture, DeveloperIsWritingReqsNotFinished) {
    spec = "type Req { unique_id : id,}";
    data = "Req ";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);
    rs_parser.ParseTokens(d_spec);

    ASSERT_THROW(ParseTokens(d_data), RdTypeException);
}

