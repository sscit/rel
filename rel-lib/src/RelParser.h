/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef RELPARSER_H_
#define RELPARSER_H_

#include "Logger.h"
#include "FileEngine.h"
#include "RsParser.h"
#include "RdParser.h"

enum class ParseResult {
    NoExceptionOccurred = 0,
    ExceptionOccurred  = 1
};

/* This class implements a workflow, which uses Lexer and Parser of
   REL specification and REL data to process a REL model.

   It requires an object of class Logger and an object of FileEngine, to hand over
   the relevant files that shall be parsed. After execution, the resulting database
   containing all requirements can be retrieved. */
class RelParser {
public:
    RelParser(Logger &l, FileEngine const &files);
    virtual ~RelParser();

    /* Req: integ_py1, integ1, integ11, integ_py1 */
    ParseResult ProcessRelModel();

    /* returns all type instances that have been read
     * Req: integ_py1
     */
    std::vector<RdFile> GetDatabase();

    /* Req: integ_py1, integ1, integ11, integ_py1 */
    static unsigned int ParseResultToInteger(ParseResult const p);

private:
    /* Req: integ_py1, integ1, integ11, integ_py1 */
    void ReadAndParseDataFiles(std::vector<FileTokenData> &);
    /* Req: integ_py1, integ1, integ11, integ_py1 */
    void ReadAndParseSpecFiles(std::vector<FileTokenData> &);
    /* Req: integ_py1, integ1, integ11, integ_py1 */
    void LexFile(FileTokenData&);

    Logger &l;
    FileEngine const &rel_model;
    RsParser rs_parser;
    RdParser rd_parser;
};

#endif
