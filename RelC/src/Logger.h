/*
 * Logger.h
 *
 *  Created on: 13.10.2020
 *      Author: Stefan_2
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <iostream>

#define LOG(level, msg) LogMessage(level, msg, __FILE__, __LINE__)

enum class LogLevel
{
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

class Logger {
public:
    Logger();
    virtual ~Logger();

    void SetLogLevel(LogLevel const l);
    LogLevel GetCurrentLogLevel() const;

    void LogMessage(LogLevel const, std::string const, std::string const, int const) const;

private:
    std::string LogLevelToString(LogLevel const l) const;

    LogLevel current_loglevel;

};

#endif /* LOGGER_H_ */
