/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef PARSER_H_
#define PARSER_H_

#include "Logger.h"
#include "AST.h"
#include "Lexer.h"

class Parser {
public:
    Parser(Logger&);
    virtual ~Parser();

protected:
    /* Req: dsl1 */
    void MultiLineComment(FileTokenData const&, std::list<Token>::const_iterator&);
    /* Req: dsl1 */
    void LineComment(FileTokenData const&, std::list<Token>::const_iterator&);
    /* Req: dsl2, dsl3, dsl4 */
    RsRdIdentifier Identifier(FileTokenData const&, std::list<Token>::const_iterator&);
    /* Returns true if the next token is of type provided, false otherwise.
     * EOLs are skipped, if there are some in between.
       Req: parser1
     */
    bool IsNextToken(FileTokenData const&, std::list<Token>::const_iterator&, TokenType const&);
    /* This method ensures that dereferencing of the iterator provided only takes place
       if the iterator is not at end() of the token list provided. If this is the case,
       then an empty token is returned.
       Req: integ1, integ11
    */
    static Token SafeDeref(FileTokenData const&, std::list<Token>::const_iterator&);

    Logger &l;
};

#endif /* PARSER_H_ */
