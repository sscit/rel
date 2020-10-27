/*
 * FileReader.h
 *
 *  Created on: 24.10.2020
 *      Author: Stefan_2
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <string>
#include <fstream>
#include <iostream>

class FileReader {
public:
    FileReader();
    FileReader(std::string);
    FileReader(FileReader const &f);
    ~FileReader();

    void OpenFile(const char*);
    bool IsFileOpen() const;
    void Close();

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
