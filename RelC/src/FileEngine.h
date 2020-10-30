/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef FILEENGINE_H_
#define FILEENGINE_H_

#include <vector>
#include <filesystem>
#include "Lexer.h"

/* Class identifies all files that shall be parsed */
class FileEngine {
public:
    FileEngine();
    virtual ~FileEngine();

    void SetSearchRecursive(bool const);
    void SetStartDirectory(std::string const);

    std::vector<FileTokenData> GetListOfFiles() const;
    bool GetSearchRecursive() const;

protected:
    void CreateFileTokenData(auto const&, std::map<std::string, DataType>&, std::vector<FileTokenData>&) const;

    bool search_recursive;
    std::string start_directory;

};

#endif /* FILEENGINE_H_ */
