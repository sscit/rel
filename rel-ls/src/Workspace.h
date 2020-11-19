/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef WORKSPACE_H_
#define WORKSPACE_H_

#include <string>
#include <iostream>
#include <map>
#include "Logger.h"
#include "RsParser.h"
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
    void UpdateFile(Uri const&, std::string const&);
    std::string& GetFileContent(Uri const&);
    void ParseTokens(FileTokenData const&);

private:
    void ParseAllFilesOnceAtStart();

    Logger &l;
    bool workspace_is_initialized;
    FileEngine file_engine;
    Uri root_uri;
    // contains the content of the files in workspace
    std::map<std::string, std::string> files;

    RsParser rs_parser;
};

#endif 
