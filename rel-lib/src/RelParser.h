/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef RELPARSER_H_
#define RELPARSER_H_

#include "Logger.h"
#include "FileEngine.h"
#include "RsParser.h"
#include "RdParser.h"

/* This class implements a workflow, which uses Lexer and Parser of
   REL specification and REL data to process a REL model.

   It requires an object of class Logger and an object of FileEngine, to hand over
   the relevant files that shall be parsed. After execution, the resulting database
   containing all requirements can be retrieved. */
class RelParser {
public:
    RelParser(Logger &l, FileEngine const &files);
    virtual ~RelParser();

    void ProcessRelModel();

private:
    void ReadAndLexInputFiles(std::vector<FileTokenData>&);
    void ParseFiles(std::vector<FileTokenData>&);

    Logger &l;
    FileEngine const &rel_model;
};

#endif
