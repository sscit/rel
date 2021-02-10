/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef RDPARSER_H_
#define RDPARSER_H_

#include <map>
#include <utility>
#include <mutex>
#include <condition_variable>
#include "Lexer.h"
#include "Token.h"
#include "ParseException.h"
#include "Logger.h"
#include "AST.h"
#include "RsParser.h"

class RdParser : public Parser {
public:
    RdParser(Logger&, RsParser const &);
    virtual ~RdParser();

    // Req: dsl1, dsl2, dsl3, dsl4, dsl5, dsl6, dsl7, dsl8
    void ParseTokens(FileTokenData const&, unsigned int const = 0);
    // Req: dsl5
    void CheckAllLinks();

    /* returns all type instances that have been read
     * Req: integ_py1
     */
    std::list<RdFile> const& GetDatabase() const;

protected:
    // Req: dsl9
    RdInteger Integer(FileTokenData const&, std::list<Token>::const_iterator&);
    // Req: dsl7, dsl6
    RdString ReadString(FileTokenData const&, std::list<Token>::const_iterator&);
    // Req: dsl2
    RdTypeInstance TypeInstance(FileTokenData const&, std::list<Token>::const_iterator&);
    // Req: dsl8
    void ParseArrayOfLinks(FileTokenData const&, std::list<Token>::const_iterator&, RdTypeInstanceAttribute&);
    // Req: integ3
    void CleanupUniqueIdDatabase(std::string const&);
    // Req: integ3
    void CleanupDatabase(std::string const&);
    // Req: integ3
    void AddUniqueIdToDatabase(RdString const&, std::string const&);
    // Req: dsl3
    bool EnumValueExists(std::vector<RsRdIdentifier> const &enum_values, RsRdIdentifier &enum_value) const;
    /* Method checks whether the attribute value identified in the data has the right
     * data type that is expected at this place
     * Req: dsl2
     */
    bool HasAttributeValueCorrectType(RsTypeAttribute const&, TokenType const);
    /* Verifies, that the token at dedicated index is of a specific type.
     * If not, throw the exception provided. EOLs are skipped, if there are
     * some.
     * Req: dsl1, dsl2, dsl3, dsl4, dsl5, dsl6, dsl7, dsl8
     */
    template<class T>
    void EnsureToken(FileTokenData const&, std::list<Token>::const_iterator&, TokenType const&, T const);
    /* Add the types generated to the database
     * Req: perf1
     */
    void AddToDatabase(RdFile const&, unsigned int const);

    // Variables for multithread support
    mutable std::mutex mtx;
    /* This variable acts as semaphore and contains the index of
       the file that shall be added next. The thread that works on
       the file with this index will add its data to the database, all other threads
       wait */
    unsigned int current_file_to_add;
    std::condition_variable db_access;

    RsParser const &specification;
    // Contains the actual data out of all .rd files
    std::list<RdFile> database;
    // Map contains all unique ids, to quickly check if an id has been used before
    std::map<std::string, RdString> unique_ids;
    std::list<TypeOrigin> unique_id_origin;
};

#endif /* RDPARSER_H_ */
