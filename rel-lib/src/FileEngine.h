/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef FILEENGINE_H_
#define FILEENGINE_H_

#include <vector>
#include <set>
#include <filesystem>
#include "Lexer.h"

bool ftd_cmp(FileTokenData const &a, FileTokenData const &b);

/* Class searches all files within a directory provided
   that shall be read and parsed */
class FileEngine {
public:
    FileEngine(Logger &);
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
    template <typename I>
    void CreateFileTokenData(I const&, std::map<std::string, DataType>&,
                             std::set<FileTokenData, decltype(ftd_cmp)*>&) const;

    bool search_recursive;
    std::string start_directory;
    Logger &l;
};

#endif /* FILEENGINE_H_ */
