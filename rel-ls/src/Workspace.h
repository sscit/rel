/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef WORKSPACE_H_
#define WORKSPACE_H_

#include <string>
#include <iostream>
#include <map>
#include "Logger.h"

// Class is used to track the state of the workspace
class Workspace {
public:
    Workspace(Logger&);
    ~Workspace();

    bool IsWorkspaceInitialized() const;
    void SetWorkspaceToInitialized();
    void UpdateFile(std::string const&, std::string const&);
    std::string& GetFileContent(std::string const&);

private:
    Logger &l;
    bool workspace_is_initialized;
    // contains the content of the files in workspace
    std::map<std::string, std::string> files;
};

#endif 
