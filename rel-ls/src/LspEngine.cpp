/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "LspEngine.h"

LspEngine::LspEngine(Logger& logger) : ws(logger), l(logger) { }
LspEngine::~LspEngine() { }



DataType LspEngine::DetermineDataType(Uri const& uri)
{
    DataType ret = DataType::Unknown;

    if (uri.IsRequirementsSpecification())
        ret = DataType::RequirementsSpecification;
    else if(uri.IsRequirementsData())
        ret = DataType::RequirementsData;

    return ret;
}

void LspEngine::HandleMessage(json const input_message) {
    if(input_message["method"] == "initialize") {
        l.LOG(LogLevel::DEBUG, "Initialize Message received");

        Uri root_uri( input_message["params"]["rootUri"] );
        // TODO what if root uri is Null?
        ws.SetWorkspaceToInitialized(root_uri);
        RespondToInitialize(input_message);
    }
    else if (input_message["method"] == "initialized") {
        l.LOG(LogLevel::DEBUG, "Initialized Message received");
    }
    else if (input_message["method"] == "textDocument/didOpen" ||
             input_message["method"] == "textDocument/didChange") {
        Uri uri (input_message["params"]["textDocument"]["uri"]);
        std::string text;

        if(input_message["method"] == "textDocument/didOpen") {
            text = input_message["params"]["textDocument"]["text"];
        }
        else {
            text = input_message["params"]["contentChanges"][0]["text"];
        }

        ws.UpdateFile(uri, text);
        l.LOG(LogLevel::DEBUG, "REL document " + uri.GetPath() + " has been opened or changed");

        ParseDocument(uri, text);
    }
}

void LspEngine::ParseDocument(Uri const& uri, std::string const& text)
{
    DataType file_type = DetermineDataType(uri);

    FileReader f(text);
    FileTokenData data(file_type, f);
    data.filepath = uri.GetPath();

    Lexer lex(l);
    lex.Read(data);

    json diag;
    Uri uri_of_file = uri;

    try {
        ws.ParseTokens(data);

        l.LOG(LogLevel::DEBUG, "REL file has been parsed, no errors");
        diag = json::array();
    }
    catch(EnumUsedButNotDefinedException &e) {
        Token const t  = e.GetToken();
        uri_of_file = Uri::CreateFileUriFromPath(t.GetFilename());
        l.LOG(LogLevel::ERROR, t.GetFilename() + ": Line " + std::to_string(t.GetLineNumberOfToken()) + ", Pos " + std::to_string(t.GetPositionInLineOfToken()) + ":");
        l.LOG(LogLevel::ERROR, e.what());
        l.LOG(LogLevel::DEBUG, "Preparing JSON diagnostic message");
        diag = CreateDiagnosticsFromException(e);
    }
    catch(ParseException &e) {
        Token const t  = e.GetToken();
        l.LOG(LogLevel::ERROR, t.GetFilename() + ": Line " + std::to_string(t.GetLineNumberOfToken()) + ", Pos " + std::to_string(t.GetPositionInLineOfToken()) + ":");
        l.LOG(LogLevel::ERROR, e.what());
        l.LOG(LogLevel::DEBUG, "Preparing JSON diagnostic message");
        diag = CreateDiagnosticsFromException(e);
    }

    json diag_message{
        { "method", "textDocument/publishDiagnostics" },
        { "params", {
            { "uri", uri_of_file.GetUri() },
            { "diagnostics", diag },
        } }
    };

    SendMessageToClient(diag_message);
}

json LspEngine::CreateDiagnosticsFromException(ParseException const& e)
{
    json diag_array = json::array();
    json diag;
    Token const t  = e.GetToken();

    json range {
        {"start", {
            { "line", t.GetLineNumberOfToken() },
            { "character", t.GetPositionInLineOfToken() },
        }},
        { "end", {
            { "line", t.GetLineNumberOfToken() },
            { "character", t.GetPositionInLineOfToken() },
        }},
    };
    diag["range"] = range;
    diag["severity"] = LogLevel::ERROR;
    diag["source"] = "rel-ls";
    diag["message"] = e.what();

    diag_array.push_back(diag);

    return diag_array;
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

        SendMessageToClient(response_message);
}

void LspEngine::SendMessageToClient(json const& message_body)
{
    json full_message = message_body;
    full_message["jsonrpc"] = "2.0";

    std::string header;
    header.append("Content-Length: " + std::to_string(full_message.dump(4).size()) + "\r\n");
    header.append("Content-Type: application/vscode-jsonrpc;charset=utf-8\r\n");
    header.append("\r\n");

    std::cout << header << full_message.dump(4);
    std::cout << std::flush;

    l.LOG(LogLevel::DEBUG, "Message sent: " + header + full_message.dump(4));
}