/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <fstream>
#include <iostream>
#include <string>

/* Class abstracts from C++ file reading interface.
   FileReader can either read from a file, or read data directly
   from string provided. */
class FileReader {
   public:
    FileReader();
    FileReader(std::string);
    FileReader(FileReader const &f);
    ~FileReader();

    /* Req: integ1, integ11 */
    void OpenFile(const char *);
    /* Req: integ1, integ11 */
    bool IsFileOpen() const;
    /* Req: integ1, integ11 */
    void Close();
    /* Req: integ1, integ11 */
    bool GetChar(char &);

   private:
    std::fstream *filestream;
    /* Is used for testing purposes, to supply data to read
     * via string instead of accessing the file system.
     */
    std::string data;
    unsigned int current_pos;
};

#endif /* FILEREADER_H_ */
