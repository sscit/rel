/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <iostream>

enum class TokenType : unsigned char {
    // reserved keywords
    TYPE,
    ENUM,
    ID,
    INT,
    STRING,
    LINK,

    // symbols and operators
    BRACKET_OPEN,
    BRACKET_CLOSE,
    COLON,
    COMMA,
    END_OF_LINE,
    QUOTATION_MARK,
    ARRAY_BEGIN,
    ARRAY_END,

    // two digit symbols
    QUOTATION_MARK_MASKED,
    LINE_COMMENT,
    COMMENT_BLOCK_START,
    COMMENT_BLOCK_END,

    // strings or integer values
    IDENTIFIER,
    STRING_VALUE,
    INTEGER_VALUE,
};
std::ostream &operator<<( std::ostream&, const TokenType&);

/* Represents a token which is an entity of the language */
class Token {
public:
    Token();
    Token(std::string const&, TokenType const&, std::string const&, unsigned int const, unsigned int const);
    ~Token();

    bool Compare(std::string const input);
    TokenType GetTokenType() const;
    std::string const * GetTokenValue() const;
    std::string const * GetFilename() const;
    unsigned int GetLineNumberOfToken() const;
    unsigned short GetPositionInLineOfToken() const;

    static std::string TokenTypeToString(TokenType const&);

private:
    std::string token_value;
    std::string const * filename;
    unsigned int line_number;
    unsigned short position_in_line;
    TokenType token_type;
};

#endif /* TOKEN_H_ */
