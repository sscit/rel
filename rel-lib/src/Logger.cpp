/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "Logger.h"

Logger::Logger() : current_loglevel(LogLevel::WARNING) { }

Logger::Logger(std::string const filename) : Logger() {
    file_access.open(filename);
}

Logger::~Logger() { 
    if(file_access.is_open())
        file_access.close();
}

void Logger::SetLogLevel(LogLevel const l) {
    current_loglevel = l;
}

LogLevel Logger::GetCurrentLogLevel() const {
    return current_loglevel;
}

std::string Logger::LogLevelToString(LogLevel const l) const {
    std::string result;
    switch(l) {
        case LogLevel::DBUG:
            result="DEBUG";
            break;
        case LogLevel::INFO:
            result="INFO";
            break;
        case LogLevel::WARNING:
            result="WARNING";
            break;
        case LogLevel::ERROR:
            result="ERROR";
            break;
        default:
            result="UNKNOWN_LOGLEVEL";
    }

    return result;
}

void Logger::LogMessage(LogLevel const loglevel, std::string const message, std::string const filename = "Unset", int const line_number = -1) {
    if (loglevel <= GetCurrentLogLevel()) {
        if(file_access.is_open()) {
            file_access << LogLevelToString(loglevel) << ": " << "File " << filename << ", Line " << line_number << ": ";
            file_access << message << std::endl;
            file_access.flush();
        }
        else {
            std::cout << LogLevelToString(loglevel) << ": " << "File " << filename << ", Line " << line_number << ": ";
            std::cout << message << std::endl;
        }
    }
}
