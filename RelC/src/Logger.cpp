/*
 * Logger.cpp
 *
 *  Created on: 13.10.2020
 *      Author: Stefan_2
 */

#include "Logger.h"

Logger::Logger() : current_loglevel(LogLevel::WARNING) {
    // TODO Auto-generated constructor stub

}

Logger::~Logger() {
    // TODO Auto-generated destructor stub
}

void Logger::SetLogLevel(LogLevel const l)
{
    current_loglevel = l;
}

LogLevel Logger::GetCurrentLogLevel() const
{
    return current_loglevel;
}

std::string Logger::LogLevelToString(LogLevel const l) const
{
    std::string result;
    switch(l)
    {
        case LogLevel::DEBUG:
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

void Logger::LogMessage(LogLevel const loglevel, std::string const message, std::string const filename = "Unset", int const line_number = -1) const
{
    if (loglevel >= GetCurrentLogLevel())
    {
        std::cout << LogLevelToString(loglevel) << ": " << "File " << filename << ", Line " << line_number << ": ";
        std::cout << message << std::endl;
    }
}
