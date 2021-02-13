/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "FileReader.h"


#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

FileReader::FileReader() : FileReader("") { }

FileReader::FileReader(std::string testdata) {
    data = testdata;
    current_pos = 0;
    filestream = new std::fstream();
}

FileReader::FileReader(FileReader const &f) : FileReader("") {
    data = f.data;
    current_pos = f.current_pos;
}

FileReader::~FileReader() {
    delete filestream;
}

void handle_error(const char* msg) {
    perror(msg); 
    exit(255);
}
const char* map_file(const char* fname, size_t& length)
{
    int fd = open(fname, O_RDONLY);
    if (fd == -1)
        handle_error("open");

    // obtain file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
        handle_error("fstat");

    length = sb.st_size;

    const char* addr = static_cast<const char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0u));
    if (addr == MAP_FAILED)
        handle_error("mmap");

    // TODO close fd at some point in time, call munmap(...)
    return addr;
}

void FileReader::OpenFile(const char* path) {
    if(data.length() == 0) {
        //filestream->open(path, std::fstream::in);
        file_ptr = map_file(path, length);
        end_of_file = file_ptr + length;
    }
}




void FileReader::Close() {
    if(data.length() == 0) {
        //filestream->close();
    }
}

bool FileReader::IsFileOpen() const {
    bool ret=true;

    if(data.length() == 0) {
        //ret = filestream->is_open();
    }
    else {
        ret = true;
    }

    return ret;
}

bool FileReader::GetChar(char &c) {
    if(data.length() == 0) {
        /*if(filestream->get(c)) {
            return true;
        }
        else return false;*/
        if(file_ptr != end_of_file)
        {
            c = *file_ptr;
            file_ptr++;
            return true;
        }
        else return false;
    }
    else {
        if(current_pos < data.size()) {
            c = data[current_pos];
            current_pos++;
            return true;
        }
        else {
            return false;
        }
    }
}
