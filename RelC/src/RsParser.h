/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef RSPARSER_H_
#define RSPARSER_H_

#include <map>
#include <utility>
#include <list>
#include "Lexer.h"
#include "Token.h"
#include "ParseException.h"
#include "Logger.h"
#include "AST.h"
#include "Parser.h"

/* class maps the URI of the file, where a 
   type or enum definition comes from, 
   to the unique name */
class TypeOrigin
{
public:
    TypeOrigin();
    TypeOrigin(std::string const&);

    std::string uri;
    std::string type_name;
};

class RsParser : public Parser {
public:
    RsParser(Logger&);
    virtual ~RsParser();

    void ParseTokens(FileTokenData const& tokens);
    void CheckAllEnumTypes();

    RsType GetType(RsRdIdentifier &type_ident) const;

protected:
    RsEnum EnumDefinition(FileTokenData const&, unsigned int &);
    void CleanupEnumDatabase(std::string const&);
    void AddEnumToDatabase(RsEnum const&, std::string const&);

    RsType TypeDefinition(FileTokenData const&, unsigned int &);
    void CleanupTypeDatabase(std::string const&);
    void AddTypeToDatabase(RsType const&, std::string const&);

    /* Returns true, if the enum value provided hasn't been defined already
       within the enum type, false otherwise */
    bool IsEnumValueUnique(RsEnum const&, RsRdIdentifier const&);

    /* Returns the filename, where a type has been defined */
    std::string FindUriOfType(RsType const&);

    /* Verifies, that the token at dedicated index is of a specific type.
     * If not, throw the exception provided. EOLs are skipped, if there are
     * some.
     */
    template<class T>
    void EnsureToken(FileTokenData const&, unsigned int&, TokenType const&, T const);

    // Key: Name of enum, Value: Enum AST node
    std::map<std::string, RsEnum> all_enums;
    std::list<TypeOrigin> enum_origin;
    // Key: Name of type, Value: Type AST node
    std::map<std::string, RsType> all_types;
    std::list<TypeOrigin> type_origin;
};

#endif /* RSPARSER_H_ */
