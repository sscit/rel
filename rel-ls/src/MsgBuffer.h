/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef MSGBUFFER_H_
#define MSGBUFFER_H_

#include <string>
#include <iostream>
#include <map>

// Class is used to build up a message, before
// the actual JSON interpretation
class MsgBuffer {
public:
    MsgBuffer();
    ~MsgBuffer();

    void AddChar(char const);
    bool IsMessageReady() const;
    std::string GetMessage() const;
    void Clear();

private:
    /* Method checks, if current_str contains a valid header 
       If yes, transfer it to map data structure. */
    void ParseHeader();
    void ExtractDataFromHeader();

    bool header_finished;

    std::string current_str;

    unsigned int message_length;
    std::string message;
    
    std::map<std::string, std::string> headers;
};

#endif 
