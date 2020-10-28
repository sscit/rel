/*
 * RdParser.h
 *
 *  Created on: 12.10.2020
 *      Author: Stefan_2
 */

#ifndef RDPARSER_H_
#define RDPARSER_H_

#include <map>
#include <utility>
#include "Lexer.h"
#include "Token.h"
#include "ParseException.h"
#include "Logger.h"
#include "AST.h"
#include "RsParser.h"

class ParsingStatistic {
public:
    ParsingStatistic() {
        number_of_files = 0;
        number_of_type_instances = 0;
        number_of_attributes = 0;
    }

    int number_of_files;
    int number_of_type_instances;
    int number_of_attributes;
};

class RdParser : public Parser {
public:
    RdParser(Logger const &, RsParser const &);
    virtual ~RdParser();

    void ParseTokens(FileTokenData const&);
    void CheckAllLinks();
    ParsingStatistic GetParsingStatistics() const;

protected:
    RdInteger Integer(FileTokenData const&, unsigned int&);
    RdString ReadString(FileTokenData const&, unsigned int&);
    RdTypeInstance TypeInstance(FileTokenData const&, unsigned int&);
    bool EnumValueExists(std::vector<RsRdIdentifier> const &enum_values, RsRdIdentifier &enum_value) const;

    /* Verifies, that the token at dedicated index is of a specific type.
     * If not, throw the exception provided. EOLs are skipped, if there are
     * some.
     */
    template<class T>
    void EnsureToken(FileTokenData const&, unsigned int&, TokenType const&, T const);

    RsParser const &specification;
    // Contains the actual data out of all .rd files
    std::vector<RdTypeInstance> database;
    // Map contains all unique ids, to quickly check if an id has been used before
    std::map<std::string, RdString> unique_ids;
    ParsingStatistic statistic;
};

#endif /* RDPARSER_H_ */
