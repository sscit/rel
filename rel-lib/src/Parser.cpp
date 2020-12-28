/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "Parser.h"
#include "ParseException.h"

Parser::Parser(Logger &logger) : l(logger) { }

Parser::~Parser() { }

RsRdIdentifier Parser::Identifier(FileTokenData const& tokens, unsigned int const &index) {
    l.LOG(LogLevel::DBUG, "Parsing Identifier");
    Token const &identifier = tokens.token_list[index];

    RsRdIdentifier data;
    data.name = identifier.GetTokenValue();

    l.LOG(LogLevel::DBUG, "Identifier " + data.name + " has been created");

    return data;
}

void Parser::LineComment(FileTokenData const& tokens, unsigned int &index) {
    l.LOG(LogLevel::DBUG, "Parsing Line Comment");

    while(tokens.token_list[index].GetTokenType() != TokenType::END_OF_LINE
          && index < tokens.token_list.size())
        index++;
}

void Parser::MultiLineComment(FileTokenData const& tokens, unsigned int &index) {
    l.LOG(LogLevel::DBUG, "Parsing Multi Line Comment");
    Token const &multi_line_comment_start = tokens.token_list[index];

    while(tokens.token_list[index].GetTokenType() != TokenType::COMMENT_BLOCK_END
          && index < tokens.token_list.size())
        index++;

    if(index == tokens.token_list.size()) {
        throw CommentException(multi_line_comment_start, "End token for Multi Line Comment not found");
    }
}

bool Parser::IsNextToken(FileTokenData const& tokens, unsigned int& index, TokenType const& tt) {
    while(tokens.token_list[index].GetTokenType() == TokenType::END_OF_LINE)
        index++;

    if(tokens.token_list[index].GetTokenType() == tt)
        return true;
    else return false;
}
