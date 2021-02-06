/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>

#define LOG(level, msg) LogMessage(level, msg, __FILE__, __LINE__)

enum class LogLevel {
    ERROR = 1,
    WARNING = 2,
    INFO = 3,
    DBUG = 4,
};

class Logger {
public:
    Logger();
    Logger(std::string const);
    virtual ~Logger();

    /* Req: parser4 */
    void SetLogLevel(LogLevel const l);
    /* Req: parser4 */
    LogLevel GetCurrentLogLevel() const;
    /* Req: parser1 */
    void LogMessage(LogLevel const, std::string const, std::string const, int const);

private:
    /* Req: parser1 */
    std::string LogLevelToString(LogLevel const l) const;

    mutable std::mutex mtx;
    std::ofstream file_access;
    LogLevel current_loglevel;
};

#endif /* LOGGER_H_ */
