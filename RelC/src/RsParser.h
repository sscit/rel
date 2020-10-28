/*
 * RsParser.h
 *
 *  Created on: 12.10.2020
 *      Author: Stefan_2
 */

#ifndef RSPARSER_H_
#define RSPARSER_H_

#include <map>
#include <utility>
#include "Lexer.h"
#include "Token.h"
#include "ParseException.h"
#include "Logger.h"
#include "AST.h"
#include "Parser.h"

class RsParser : public Parser {
public:
    RsParser(Logger const &);
    virtual ~RsParser();

    void ParseTokens(FileTokenData const& tokens);
    void CheckAllEnumTypes();

    RsType GetType(RsRdIdentifier &type_ident) const;

protected:
    RsEnum EnumDefinition(FileTokenData const&, unsigned int &);
    RsType TypeDefinition(FileTokenData const&, unsigned int &);

    /* Returns true, if the enum value provided hasn't been defined already
       within the enum type, false otherwise */
    bool IsEnumValueUnique(RsEnum const&, RsRdIdentifier const&);

    /* Verifies, that the token at dedicated index is of a specific type.
     * If not, throw the exception provided. EOLs are skipped, if there are
     * some.
     */
    template<class T>
    void EnsureToken(FileTokenData const&, unsigned int&, TokenType const&, T const);

    // Key: Name of enum, Value: Enum AST node
    std::map<std::string, RsEnum> all_enums;
    // Key: Name of type, Value: Type AST node
    std::map<std::string, RsType> all_types;
};

#endif /* RSPARSER_H_ */
