/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef FILEENGINE_H_
#define FILEENGINE_H_

#include <vector>
#include <set>
#include <filesystem>
#include "Lexer.h"

bool ftd_cmp(FileTokenData const &a, FileTokenData const &b);

/* Class searches for all relevant files within all directories provided,
   that shall be read and parsed */
class FileEngine {
public:
    FileEngine(Logger &);
    virtual ~FileEngine();

    /* If set to true, the search runs recursive, starting
       from the directory provided */
    void SetSearchRecursive(bool const);
    /* Set a directory, where the FileEngine shall
       search for relevant files */
    void SetDirectory(std::string const);
    /* Returns a vector of FileTokenData, each element containing
       a path to a relevant file, that shall be parsed */
    std::vector<FileTokenData> GetListOfFiles() const;
    bool GetSearchRecursive() const;

protected:
   template<typename T>
    void CreateFileTokenData(T const&, std::map<std::string, DataType>&,
                             std::set<FileTokenData, decltype(ftd_cmp)*>&) const;

    bool search_recursive;
    std::vector<std::string> directories;
    Logger &l;
};

#endif /* FILEENGINE_H_ */
