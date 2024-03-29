/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "Token.h"

#include <string.h>

// Operator Definition, to convert the token identifiers into strings
std::ostream &operator<<(std::ostream &output, const TokenType &t) {
    output << Token::TokenTypeToString(t);

    return output;
}

Token::Token() : filename(nullptr) {}

Token::Token(std::string const &tv, TokenType const &t,
             std::string const &fname, unsigned int const current_line,
             unsigned int const current_pos_in_line) {
    token_type = t;
    token_value = tv;
    filename = &fname;
    line_number = current_line;
    position_in_line = current_pos_in_line;
}

/* Compare if the string provided is equal to
 * this token.
 */
bool Token::Compare(std::string const input) {
    if (token_value.compare(input) == 0) {
        return true;
    } else
        return false;
}

TokenType Token::GetTokenType() const { return token_type; }

std::string Token::TokenTypeToString(TokenType const &tt) {
    std::string output;

    switch (tt) {
        case TokenType::TYPE:
            output = "type";
            break;

        case TokenType::ENUM:
            output = "enum";
            break;

        case TokenType::ID:
            output = "id";
            break;

        case TokenType::INT:
            output = "int";
            break;

        case TokenType::STRING:
            output = "string";
            break;

        case TokenType::LINK:
            output = "link";
            break;

        case TokenType::LINE_COMMENT:
            output = "line comment";
            break;

        case TokenType::COMMENT_BLOCK_START:
            output = "comment start";
            break;

        case TokenType::COMMENT_BLOCK_END:
            output = "comment end";
            break;

        case TokenType::BRACKET_OPEN:
            output = "bracket op";
            break;

        case TokenType::BRACKET_CLOSE:
            output = "bracket cl";
            break;

        case TokenType::COLON:
            output = "colon";
            break;

        case TokenType::COMMA:
            output = "comma";
            break;

        case TokenType::END_OF_LINE:
            output = "eol";
            break;

        case TokenType::QUOTATION_MARK:
            output = "quotation mark";
            break;

        case TokenType::QUOTATION_MARK_MASKED:
            output = "quotation mark masked";
            break;

        case TokenType::ARRAY_BEGIN:
            output = "start of array [";
            break;

        case TokenType::ARRAY_END:
            output = "end of array ]";
            break;

        case TokenType::IDENTIFIER:
            output = "identifier";
            break;

        case TokenType::STRING_VALUE:
            output = "string value";
            break;

        case TokenType::INTEGER_VALUE:
            output = "integer value";
            break;
    }

    return output;
}

std::string const *Token::GetTokenValue() const { return &token_value; }

std::string const *Token::GetFilename() const { return filename; }

unsigned int Token::GetLineNumberOfToken() const { return line_number; }

unsigned short Token::GetPositionInLineOfToken() const {
    return position_in_line;
}

Token::~Token() {}
