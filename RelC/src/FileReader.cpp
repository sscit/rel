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
    eof = new std::fstream();
    valid = new std::fstream();
    filestream = new std::fstream();

    eof->setstate(std::ios_base::eofbit);
}

FileReader::FileReader(FileReader const &f) : FileReader("")
{
    data = f.data;
    current_pos = f.current_pos;
}

FileReader::~FileReader() {
    delete eof;
    delete valid;
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

std::istream& FileReader::GetChar(char &c)
{
    if(data.length() == 0)
    {
        return filestream->get(c);
    }
    else
    {
        if(current_pos < data.size())
        {
            c = data[current_pos];
            current_pos++;
            return *valid;
        }
        else
        {
            return *eof;
        }
    }
}



