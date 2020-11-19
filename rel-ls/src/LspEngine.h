/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef LSPENGINE_H_
#define LSPENGINE_H_

#include <string>
#include <iostream>
#include <map>
#include "nlohmann/json.hpp"
#include "Lexer.h"
#include "RsParser.h"
#include "RdParser.h"
#include "Logger.h"

#include "Workspace.h"
#include "Uri.h"

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
    // make a full message out of the json data provided and
    // send it out
    void SendMessageToClient(json const&);
    void ParseDocument(Uri const&, std::string const&);
    json CreateDiagnosticsFromException(ParseException const&);
    // Determine type of data out of the uri provided
    static DataType DetermineDataType(Uri const&);

    Workspace ws;
    Logger &l;
};

#endif 
