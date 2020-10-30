/*
 * Parser.cpp
 *
 *  Created on: 15.10.2020
 *      Author: Stefan_2
 */

#include "Parser.h"
#include "ParseException.h"

Parser::Parser(Logger const &logger) : l(logger) {
    // TODO Auto-generated constructor stub

}

Parser::~Parser() {
    // TODO Auto-generated destructor stub
}

RsRdIdentifier Parser::Identifier(FileTokenData const& tokens, unsigned int const &index)
{
    l.LOG(LogLevel::DEBUG, "Parsing Identifier");
    Token const &identifier = tokens.token_list[index];

    RsRdIdentifier data;
    data.name = identifier.GetTokenValue();

    l.LOG(LogLevel::DEBUG, "Identifier " + data.name + " has been created");

    return data;
}

void Parser::LineComment(FileTokenData const& tokens, unsigned int &index)
{
    l.LOG(LogLevel::DEBUG, "Parsing Line Comment");
    Token const &line_comment_start = tokens.token_list[index];

    while(tokens.token_list[index].GetTokenType() != TokenType::END_OF_LINE
          && index < tokens.token_list.size())
        index++;

    if(index == tokens.token_list.size())
    {
        throw CommentException(line_comment_start);
    }
}

void Parser::MultiLineComment(FileTokenData const& tokens, unsigned int &index)
{
    l.LOG(LogLevel::DEBUG, "Parsing Multi Line Comment");
    Token const &multi_line_comment_start = tokens.token_list[index];

    while(tokens.token_list[index].GetTokenType() != TokenType::COMMENT_BLOCK_END
          && index < tokens.token_list.size())
        index++;

    if(index == tokens.token_list.size())
    {
        throw CommentException(multi_line_comment_start, "End token for Multi Line Comment not found");
    }
}

bool Parser::IsNextToken(FileTokenData const& tokens, unsigned int& index, TokenType const& tt)
{
    while(tokens.token_list[index].GetTokenType() == TokenType::END_OF_LINE)
        index++;

    if(tokens.token_list[index].GetTokenType() == tt)
        return true;
    else return false;
}
