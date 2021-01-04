/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "Cli.h"

/* main method of rel_cli, the command line interface for REL
   parser */
int main(int argc, char* argv[]) {
    Cli rel_cli;

    return rel_cli.Run(argc, argv);
}
