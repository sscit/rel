/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef PARSER_H_
#define PARSER_H_

#include "Logger.h"
#include "AST.h"
#include "Lexer.h"

class Parser {
public:
    Parser(Logger const &);
    virtual ~Parser();

protected:
    void MultiLineComment(FileTokenData const&, unsigned int&);
    void LineComment(FileTokenData const&, unsigned int&);
    RsRdIdentifier Identifier(FileTokenData const&, unsigned int const&);
    /* Returns true if the next token is of type provided, false otherwise.
     * EOLs are skipped, if there are some in between.
     */
    bool IsNextToken(FileTokenData const&, unsigned int&, TokenType const&);

    Logger const &l;
};

#endif /* PARSER_H_ */
