/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef FILEENGINE_H_
#define FILEENGINE_H_

#include <vector>
#include <filesystem>
#include "Lexer.h"

/* Class searches all files within a directory provided
   that shall be read and parsed */
class FileEngine {
public:
    FileEngine();
    virtual ~FileEngine();

    /* If set to true, the search runs recursive, starting
       from the directory provided */
    void SetSearchRecursive(bool const);
    /* Set the start directory, where the FileEngine shall
       start searching for relevant files */
    void SetStartDirectory(std::string const);
    /* Returns a vector of FileTokenData, each element containing
       a path to a relevant file, that shall be parsed */
    std::vector<FileTokenData> GetListOfFiles() const;
    bool GetSearchRecursive() const;

protected:
    void CreateFileTokenData(auto const&, std::map<std::string, DataType>&, std::vector<FileTokenData>&) const;

    bool search_recursive;
    std::string start_directory;

};

#endif /* FILEENGINE_H_ */
