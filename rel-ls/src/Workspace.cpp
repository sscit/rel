/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "Workspace.h"

Workspace::Workspace(Logger& logger) : l(logger), workspace_is_initialized(false), file_engine(logger),
                                       rs_parser(logger), rd_parser(logger, rs_parser) { }
Workspace::~Workspace() { }

void Workspace::ParseTokens(FileTokenData const& tokens) {
    if(tokens.GetDataTypeOfTokenList() == DataType::RequirementsSpecification) {
        rs_parser.ParseTokens(tokens);
        rs_parser.CheckAllEnumTypes();
    }
    else if(tokens.GetDataTypeOfTokenList() == DataType::RequirementsData) {
        rd_parser.ParseTokens(tokens);
        rd_parser.CheckAllLinks();
    }
}

bool Workspace::IsWorkspaceInitialized() const {
    return workspace_is_initialized;
}

void Workspace::SetWorkspaceToInitialized(Uri const& uri) {
    workspace_is_initialized = true;
    root_uri = uri;

    ParseAllFilesOnceAtStart();
}

void Workspace::ParseAllFilesOnceAtStart()
{
    // Parse relevant files in workspace at startup, to create the data structures
    file_engine.SetSearchRecursive(true);
    file_engine.SetStartDirectory(root_uri.GetPath());

    l.LOG(LogLevel::DBUG, "Looking for files starting from " + root_uri.GetPath());
    std::vector<FileTokenData> input_files;
    input_files = file_engine.GetListOfFiles();

    for(unsigned int i=0; i<input_files.size(); i++) {
        l.LOG(LogLevel::INFO, "Reading file " + input_files[i].filepath + " and creating tokens");
        Lexer lex(l);
        lex.Read(input_files[i]);
    }

    // Parse the specifications and build up the data structures
    std::vector<FileTokenData>::iterator iter = input_files.begin();
    while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsSpecification);})) != input_files.end() ) {
        l.LOG(LogLevel::INFO, "Parsing tokens from " + iter->filepath);
        try {
            rs_parser.ParseTokens(*iter);
        }
        catch(const std::exception& e) {
            /* do not handle errors at this point, just continue
               with the next file */
        }
        iter++;
    }

    try {
        rs_parser.CheckAllEnumTypes();
    }
    catch(const std::exception& e) {
            /* do not handle errors at this point, just continue
               with the next file */
    }

    // Now parse the data and build up data structures
    iter = input_files.begin();
    while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsData);})) != input_files.end() ) {
        l.LOG(LogLevel::INFO, "Parsing of tokens from " + iter->filepath);
        try {
            rd_parser.ParseTokens(*iter);
        }
        catch(const std::exception& e) {
            /* do not handle errors at this point, just continue
               with the next file */
        }
        iter++;
    }
}

void Workspace::UpdateFile(Uri const& uri, std::string const& file_content) {
    files[uri.GetPath()] = file_content;
}

std::string& Workspace::GetFileContent(Uri const& uri) {
    return files[uri.GetPath()];
}
