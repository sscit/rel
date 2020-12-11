/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <iostream>

enum class TokenType {
    // reserved keywords
    TYPE,
    ENUM,
    ID,
    INT,
    STRING,
    LINK,

    // two digit symbols
    LINE_COMMENT,
    COMMENT_BLOCK_START,
    COMMENT_BLOCK_END,

    // symbols and operators
    BRACKET_OPEN,
    BRACKET_CLOSE,
    COLON,
    COMMA,
    END_OF_LINE,
    QUOTATION_MARK,
    QUOTATION_MARK_MASKED,

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
    bool Compare(std::string const input);
    virtual ~Token();
    TokenType GetTokenType() const;
    std::string GetTokenValue() const;
    std::string GetFilename() const;
    int GetLineNumberOfToken() const;
    int GetPositionInLineOfToken() const;

    static std::string TokenTypeToString(TokenType const&);

private:
    std::string token_value;
    TokenType token_type;

    std::string filename;
    unsigned int line_number;
    unsigned int position_in_line;
};

#endif /* TOKEN_H_ */
