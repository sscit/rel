/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "Parser.h"
#include "ParseException.h"

Parser::Parser(Logger &logger) : l(logger) { }

Parser::~Parser() { }

RsRdIdentifier Parser::Identifier(FileTokenData const& tokens, std::list<Token>::const_iterator& iter) {
    l.LOG(LogLevel::DBUG, "Parsing Identifier");
    Token const &identifier = *iter;

    RsRdIdentifier data;
    data.name = *identifier.GetTokenValue();

    l.LOG(LogLevel::DBUG, "Identifier " + data.name + " has been created");

    return data;
}

void Parser::LineComment(FileTokenData const& tokens, std::list<Token>::const_iterator& iter) {
    l.LOG(LogLevel::DBUG, "Parsing Line Comment");

    while(iter != tokens.token_list.end() &&
          iter->GetTokenType() != TokenType::END_OF_LINE)
        iter++;

    l.LOG(LogLevel::DBUG, "Parsing Line Comment Finished");
}

void Parser::MultiLineComment(FileTokenData const& tokens, std::list<Token>::const_iterator& iter) {
    l.LOG(LogLevel::DBUG, "Parsing Multi Line Comment");
    Token const &multi_line_comment_start = *iter;

    while(iter->GetTokenType() != TokenType::COMMENT_BLOCK_END
          && iter != tokens.token_list.end())
        iter++;

    if(iter == tokens.token_list.end()) {
        throw CommentException(multi_line_comment_start, "End token for Multi Line Comment not found");
    }
}

bool Parser::IsNextToken(FileTokenData const& tokens, std::list<Token>::const_iterator& iter, TokenType const& tt) {
    while(iter->GetTokenType() == TokenType::END_OF_LINE)
        iter++;

    if(iter->GetTokenType() == tt)
        return true;
    else return false;
}

Token Parser::SafeDeref(FileTokenData const& tokens, std::list<Token>::const_iterator& iter) {
    Token ret;

    if(iter != tokens.token_list.end())
        return *iter;

    return ret;
}
