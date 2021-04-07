/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef CLI_H_
#define CLI_H_

#include <chrono>
#include <iostream>
#include <thread>

#include "FileEngine.h"
#include "Logger.h"
#include "RelParser.h"

/* main class, implementing CLI interface for rel-lib library */
class Cli {
   public:
    Cli();
    ~Cli();

    int Run(int, char*[]);

   protected:
    void ProcessCommandLine(int const, char const* const[], FileEngine&,
                            Logger&) const;
};

#endif
