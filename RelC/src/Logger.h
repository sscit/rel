/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <iostream>
#include <fstream>

#define LOG(level, msg) LogMessage(level, msg, __FILE__, __LINE__)

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

class Logger {
public:
    Logger();
    Logger(std::string const);
    virtual ~Logger();

    void SetLogLevel(LogLevel const l);
    LogLevel GetCurrentLogLevel() const;

    void LogMessage(LogLevel const, std::string const, std::string const, int const);

private:
    std::string LogLevelToString(LogLevel const l) const;
    std::ofstream file_access;
    LogLevel current_loglevel;
};

#endif /* LOGGER_H_ */
