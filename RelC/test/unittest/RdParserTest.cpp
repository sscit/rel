#include "gtest/gtest.h"
#include "RelC/src/RdParser.h"

class RdParserTestFixture : public ::testing::Test, public RdParser {
protected:
  RdParserTestFixture() : RdParser(logger, rs_parser), lexer_test(logger), rs_parser(logger) { }

  void SetUp() override { }

  void TearDown() override { }

  Logger logger;
  std::string spec;
  std::string data;
  Lexer lexer_test;
  RsParser rs_parser;
};

TEST_F(RdParserTestFixture, SingleDataset) {
    spec = "type Req { unique_id : id, Text : string,}";
    data = "Req { unique_id : \"id1\", Text : \"First Requirement\",}";

    FileReader r_spec(spec);
    FileTokenData d_spec(DataType::RequirementsSpecification, r_spec);

    FileReader r_data(data);
    FileTokenData d_data(DataType::RequirementsData, r_data);

    lexer_test.Read(d_spec);
    lexer_test.Read(d_data);   
    rs_parser.ParseTokens(d_spec);
        
    ParseTokens(d_data);

    EXPECT_EQ(database.size(), 1);
    EXPECT_EQ(database[0].type_elements_data.size(), 2);
}
