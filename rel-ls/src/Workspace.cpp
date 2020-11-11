/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "Workspace.h"

Workspace::Workspace(Logger& logger) : l(logger), workspace_is_initialized(false) { }
Workspace::~Workspace() { }

bool Workspace::IsWorkspaceInitialized() const {
    return workspace_is_initialized;
}

void Workspace::SetWorkspaceToInitialized() {
    workspace_is_initialized = true;
}

void Workspace::UpdateFile(std::string const& uri, std::string const& file_content)
{
    files[uri] = file_content;
}

std::string& Workspace::GetFileContent(std::string const& uri)
{
    return files[uri];
}
