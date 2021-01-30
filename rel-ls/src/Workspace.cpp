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

FileTokenData& Workspace::GetFileHandler(Uri const &uri) {
    for(auto &f : input_files) {
        if(uri.GetPath().compare(f.filepath) == 0) {
            return f;
        }
    }

    /* File was not found, probably is was added after initial
       scan. Therefore add a new entry to workspace */

    FileTokenData new_file(uri.GetDataType());
    new_file.filepath = uri.GetPath();
    input_files.push_back(new_file);

    return input_files.back();
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
    file_engine.SetDirectory(root_uri.GetPath());

    l.LOG(LogLevel::DBUG, "Looking for files starting from " + root_uri.GetPath());
    input_files = file_engine.GetListOfFiles();

    try {
        ReadAndParseSpecFiles();
        ReadAndParseDataFiles();
    }
    catch (FileIoException &e) {
        /* do not handle errors at this time*/
    }
    catch(ParseException &e) {
        /* do not handle errors at this time*/
    }
}

void Workspace::LexFile(FileTokenData& file) {
    l.LOG(LogLevel::INFO, "Reading file " + file.filepath + " and creating tokens");
    Lexer lex(l);
    lex.Read(file);
}

void Workspace::ReadAndParseDataFiles() {
    std::vector<FileTokenData>::iterator iter = input_files.begin();
    while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsData);})) != input_files.end() ) {
        LexFile(*iter);

        l.LOG(LogLevel::INFO, "Parsing of tokens from " + iter->filepath);
        rd_parser.ParseTokens(*iter);
        // delete the tokens, they are not needed anymore
        iter->token_list.clear();
        iter++;
    }

    // check all links
    rd_parser.CheckAllLinks();
}

void Workspace::ReadAndParseSpecFiles() {
    /* Lex and parse the specification files, to build up the spec */
    std::vector<FileTokenData>::iterator iter = input_files.begin();
    while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsSpecification);})) != input_files.end() ) {
        LexFile(*iter);

        l.LOG(LogLevel::INFO, "Parsing tokens from " + iter->filepath);
        rs_parser.ParseTokens(*iter);
        // delete the tokens, they are not needed anymore
        iter->token_list.clear();
        iter++;
    }

    // Ensure, that all enum types used exist
    rs_parser.CheckAllEnumTypes();
}


