/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef WORKSPACE_H_
#define WORKSPACE_H_

#include <string>
#include <iostream>
#include <map>
#include "Logger.h"
#include "RsParser.h"
#include "RdParser.h"
#include "FileEngine.h"

#include "Uri.h"

/* Class is used to track the state of the workspace,
   its files and the parsers
*/
class Workspace {
public:
    Workspace(Logger&);
    ~Workspace();

    bool IsWorkspaceInitialized() const;
    void SetWorkspaceToInitialized(Uri const&);
    void ParseTokens(FileTokenData const&);
    /* Returns Workspace's data structure to handle the
     * file identified by the Uri
     */
    FileTokenData& GetFileHandler(Uri const &);
    /* returns true if the position provided represents a link, false otherwise.
       If true, then the target of the link is returned via parameter.
     */
    bool GetTargetOfLink(std::string const&, IdentifierPosition const&, IdentifierPosition&, std::string&);

private:
    void ParseAllFilesOnceAtStart();
    void LexFile(FileTokenData&);
    void ReadAndParseDataFiles();
    void ReadAndParseSpecFiles();

    Logger &l;
    bool workspace_is_initialized;
    FileEngine file_engine;
    Uri root_uri;
    std::vector<FileTokenData> input_files;

    RsParser rs_parser;
    RdParser rd_parser;
};

#endif 
