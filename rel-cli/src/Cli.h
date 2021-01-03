/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef CLI_H_
#define CLI_H_

#include <iostream>
#include <thread>
#include <chrono>
#include "FileEngine.h"
#include "RelParser.h"
#include "Logger.h"

/* main class, implementing CLI interface for rel-lib library */
class Cli {
public:
    Cli();
    ~Cli();
    
    int Run(int, char* []);

protected:
    void ProcessCommandLine(int const, char const * const[], FileEngine&, Logger&) const;
};

#endif
