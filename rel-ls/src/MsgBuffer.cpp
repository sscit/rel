/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "MsgBuffer.h"

MsgBuffer::MsgBuffer(Logger& logger)
    : header_finished(false), message_length(0), l(logger) {}
MsgBuffer::~MsgBuffer() {}

void MsgBuffer::Clear() {
    message_length = 0;
    message.clear();
    current_str.clear();

    header_finished = false;
    headers.clear();
}

bool MsgBuffer::IsMessageReady() const {
    bool ret = true;

    if (header_finished == false)
        ret = false;
    else if (message.length() < message_length) {
        ret = false;
    } else
        ret = true;

    return ret;
}

std::string MsgBuffer::GetMessage() const { return message; }

void MsgBuffer::ParseHeader() {
    if (current_str == "\r\n") {
        header_finished = true;
        l.LOG(LogLevel::DBUG, "Header has been parsed");
    } else {
        auto eol_pos = current_str.find("\r\n");
        if (eol_pos != std::string::npos) {
            std::string tuple = current_str.substr(0, eol_pos);
            auto delim_pos = tuple.find(":");
            if (delim_pos != std::string::npos) {
                std::string key = tuple.substr(0, delim_pos);
                std::string value = tuple.substr(delim_pos + 1);
                headers[key] = value;
                l.LOG(LogLevel::DBUG, "Header key: " + key);
                l.LOG(LogLevel::DBUG, "Header value: " + value);
            }
            current_str.clear();
            ExtractDataFromHeader();
        }
    }
}

void MsgBuffer::ExtractDataFromHeader() {
    auto search = headers.find("Content-Length");
    if (search != headers.end()) {
        message_length = std::atoi(search->second.c_str());
    }
}

void MsgBuffer::AddChar(char const c) {
    if (header_finished) {
        message.push_back(c);
    } else {
        current_str.push_back(c);
        ParseHeader();
    }
}