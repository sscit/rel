#include "gtest/gtest.h"
#include "rel-lib/src/Parser.h"

class ParserTestFixture : public ::testing::Test, public Parser {
   protected:
    ParserTestFixture() : Parser(logger) {}

    void SetUp() override {
        empty_string = "";
        testdata_rd = new FileTokenData(DataType::RequirementsData);
        testdata_rs = new FileTokenData(DataType::RequirementsSpecification);
        eol = new Token("\n", TokenType::END_OF_LINE, empty_string, 0, 0);
        random_string = new Token("dhasjhdkas_dasdsa", TokenType::STRING_VALUE,
                                  empty_string, 0, 0);
    }

    void TearDown() override {
        delete testdata_rs;
        delete testdata_rd;
        delete eol;
    }

    Logger logger;
    FileTokenData *testdata_rs;
    FileTokenData *testdata_rd;
    Token *eol;
    Token *random_string;
    std::string empty_string;
};

TEST_F(ParserTestFixture, IsNextTokenCorrectlyIdentified) {
    for (int i = 0; i < 3; i++) testdata_rs->token_list.push_back(*eol);

    Token colon("", TokenType::COLON, empty_string, 0, 0);
    testdata_rs->token_list.push_back(colon);

    std::list<Token>::const_iterator iter = testdata_rs->token_list.begin();
    EXPECT_TRUE(IsNextToken(*testdata_rs, iter, TokenType::COLON));
    iter = testdata_rs->token_list.begin();
    EXPECT_FALSE(IsNextToken(*testdata_rs, iter, TokenType::ID));
}

TEST_F(ParserTestFixture, IsNextTokenCorrectlyIdentified2) {
    Token enumx("//", TokenType::ENUM, empty_string, 0, 0);
    testdata_rs->token_list.push_back(enumx);

    std::list<Token>::const_iterator iter = testdata_rs->token_list.begin();
    EXPECT_TRUE(IsNextToken(*testdata_rs, iter, TokenType::ENUM));
    iter = testdata_rs->token_list.begin();
    EXPECT_FALSE(IsNextToken(*testdata_rs, iter, TokenType::BRACKET_CLOSE));
}

TEST_F(ParserTestFixture, MultiLineCommentRead) {
    int tokens_added = 0;
    Token comment_start("/*", TokenType::COMMENT_BLOCK_START, empty_string, 0,
                        0);
    testdata_rs->token_list.push_back(comment_start);
    tokens_added++;
    for (; tokens_added < 15; tokens_added++) {
        if (tokens_added % 2 == 0)
            testdata_rs->token_list.push_back(*random_string);
        else {
            testdata_rs->token_list.push_back(*eol);
        }
    }

    Token comment_end("*/", TokenType::COMMENT_BLOCK_END, empty_string, 0, 0);
    testdata_rs->token_list.push_back(comment_end);
    tokens_added++;
    for (int i = 0; i < 1; i++)
        testdata_rs->token_list.push_back(*random_string);

    std::list<Token>::const_iterator iter = testdata_rs->token_list.begin();
    MultiLineComment(*testdata_rs, iter);
    iter++;
    iter++;
    EXPECT_EQ(testdata_rs->token_list.end(), iter);
}

TEST_F(ParserTestFixture, ReadIdentifier) {
    Token ident("MyIdentifier", TokenType::IDENTIFIER, empty_string, 0, 0);
    testdata_rs->token_list.push_back(ident);

    std::list<Token>::const_iterator iter = testdata_rs->token_list.begin();
    RsRdIdentifier r = Identifier(*testdata_rs, iter);

    EXPECT_EQ(r.name, *ident.GetTokenValue());
}

TEST_F(ParserTestFixture, LineCommentRead) {
    int tokens_added = 0;
    Token linecomment("", TokenType::LINE_COMMENT, empty_string, 0, 0);
    testdata_rs->token_list.push_back(linecomment);
    tokens_added++;
    for (; tokens_added < 5; tokens_added++)
        testdata_rs->token_list.push_back(*random_string);

    testdata_rs->token_list.push_back(*eol);
    tokens_added++;
    for (int i = 0; i < 1; i++)
        testdata_rs->token_list.push_back(*random_string);

    std::list<Token>::const_iterator iter = testdata_rs->token_list.begin();
    LineComment(*testdata_rs, iter);
    iter++;
    iter++;
    EXPECT_EQ(testdata_rs->token_list.end(), iter);
}