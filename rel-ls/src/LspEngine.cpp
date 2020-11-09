/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "LspEngine.h"

LspEngine::LspEngine(Logger& logger) : ws(logger), l(logger) { }
LspEngine::~LspEngine() { }

void LspEngine::HandleMessage(json const input_message) {
    if(input_message["method"] == "initialize") {
        l.LOG(LogLevel::DEBUG, "Initialize Message received");
        
        ws.SetWorkspaceToInitialized();
        RespondToInitialize(input_message);
    }
    else if (input_message["method"] == "initialized") {
        l.LOG(LogLevel::DEBUG, "Initialized Message received");
    }
}

void LspEngine::RespondToInitialize(json const &input_message) {
        json const text_document_sync_options {
            { "openClose", true },
            { "change", 1 }, // TextDocumentSyncKind.Full
            { "willSave", false },
            { "willSaveWaitUntil", false },
            { "save", { { "includeText", false } } },
        };

        json const completion_options {
            { "resolveProvider", true },
            { "triggerCharacters", {} },
        };

        json const initialize_result {
            {
                "capabilities",
                {
                { "textDocumentSync", text_document_sync_options },
                { "completionProvider", completion_options },
                }
            }
        };

        json const response_message {
            { "id", input_message["id"] },
            { "result", initialize_result }
        };

        SendResponse(response_message);
}

void LspEngine::SendResponse(json const& message_body)
{
    json full_message = message_body;
    full_message["jsonrpc"] = "2.0";

    std::string header;
    header.append("Content-Length: " + std::to_string(full_message.dump(4).size()) + "\r\n");
    header.append("Content-Type: application/vscode-jsonrpc;charset=utf-8\r\n");
    header.append("\r\n");

    std::cout << header << full_message.dump(4);
    std::cout << std::flush;

    l.LOG(LogLevel::DEBUG, "Init Response sent: " + header + full_message.dump(4));
}