/*
 * FileReader.cpp
 *
 *  Created on: 24.10.2020
 *      Author: Stefan_2
 */

#include "FileReader.h"

FileReader::FileReader() : FileReader("") {

}

FileReader::FileReader(std::string testdata)
{
    data = testdata;
    current_pos = 0;
    filestream = new std::fstream();
}

FileReader::FileReader(FileReader const &f) : FileReader("")
{
    data = f.data;
    current_pos = f.current_pos;
}

FileReader::~FileReader() {
    delete filestream;
}


void FileReader::OpenFile(const char* path)
{
    if(data.length() == 0)
    {
        filestream->open(path);
    }
}

void FileReader::Close()
{
    if(data.length() == 0)
    {
        filestream->close();
    }
}

bool FileReader::IsFileOpen() const
{
    bool ret;

    if(data.length() == 0)
    {
        ret = filestream->is_open();
    }
    else
    {
        ret = true;
    }

    return ret;
}

bool FileReader::GetChar(char &c)
{
    if(data.length() == 0)
    {
        if(filestream->get(c))
        {
            return true;
        }
        else return false;
    }
    else
    {
        if(current_pos < data.size())
        {
            c = data[current_pos];
            current_pos++;
            return true;
        }
        else
        {
            return false;
        }
    }
}



