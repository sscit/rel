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

/* Represent the exact location
 * of an identifier within a file
 */
class IdentifierPosition
{
public:
    IdentifierPosition() : line_number(0), position_in_line(0), length(0) {}
    IdentifierPosition(std::string const&, unsigned int const, unsigned short const, unsigned short const);

    std::string identifier_name;
    unsigned int line_number;
    unsigned short position_in_line;
    unsigned short length;
};

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
    /* returns true if the position provided represents a link, false otherwise.
       If true, then the target of the link is returned via parameters.
     */
    bool GetTargetOfLink(std::string const&, IdentifierPosition const&, IdentifierPosition&, std::string&);

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
    void AddUniqueIdToDatabase(RdString const&, std::string const&, Token const&);
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
    /* Add the link's position in file to the data structure 
     * Req: 
     */
    void StoreLinkLocationInFile(std::string const&, RsRdIdentifier const&, Token const&);

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
    /* Map contains all unique ids and their position,
     * to quickly query ids, e.g. to check if they exist already
     */
    std::map<std::string, IdentifierPosition> unique_ids;
    std::list<TypeOrigin> unique_id_origin;

    // map contains all link's positions in a file. Key is filename
    std::map<std::string, std::vector<IdentifierPosition> > links;
};

#endif /* RDPARSER_H_ */
