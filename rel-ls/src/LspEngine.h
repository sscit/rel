/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef LSPENGINE_H_
#define LSPENGINE_H_

#include <string>
#include <iostream>
#include <map>
#include "nlohmann/json.hpp"
#include "Logger.h"
#include "Workspace.h"

using namespace nlohmann;

// Class is used to handle Lsp protocol
class LspEngine {
public:
    LspEngine(Logger&);
    ~LspEngine();

    void HandleMessage(json const);

private:
    // Method prepares the response for initialization
    void RespondToInitialize(json const&);
    // make a full method out of the json data provided
    void SendResponse(json const&);

    Workspace ws;
    Logger &l;
};

#endif 
