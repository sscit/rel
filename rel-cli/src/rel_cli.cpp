/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include <iostream>
#include <thread>
#include <chrono>
#include "FileEngine.h"
#include "RelParser.h"
#include "Logger.h"

using namespace std;

/* Method Definitions */
void ProcessCommandLine(int const argc, char const * const argv[], FileEngine &f, Logger &l) {
    std::string root_dir(".");

    if(argc > 1) {
        for(int i=1; i<argc; ++i) {
            std::string argument(argv[i]);

            if(argument.compare("-r") == 0)
                f.SetSearchRecursive(true);
            else if(argument.compare("-v") == 0)
                l.SetLogLevel(LogLevel::INFO);
            else if(argument.compare("-vv") == 0)
                l.SetLogLevel(LogLevel::DEBUG);
            else
                f.SetStartDirectory(argument);
        }
    }
}

/* main method of rel_cli, the command line interface for REL
   parser */
int main(int argc, char* argv[]) {
    auto t1 = std::chrono::high_resolution_clock::now();

    /* Initialize singletons */
    Logger logger_object;
    FileEngine input_file_handler(logger_object);
    ProcessCommandLine(argc, argv, input_file_handler, logger_object);

    RelParser rel(logger_object, input_file_handler);
    ParseResult res = rel.ProcessRelModel();

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    
    logger_object.LOG(LogLevel::INFO, "Execution Time: " + std::to_string(duration) + " microseconds");

    return RelParser::ParseResultToInteger(res);
}
