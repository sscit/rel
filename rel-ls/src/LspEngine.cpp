/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "LspEngine.h"

LspEngine::LspEngine(Logger& logger) : ws(logger), l(logger) { }
LspEngine::~LspEngine() { }

DataType LspEngine::DetermineDataType(Uri const& uri) {
    DataType ret = DataType::Unknown;

    if (uri.IsRequirementsSpecification())
        ret = DataType::RequirementsSpecification;
    else if(uri.IsRequirementsData())
        ret = DataType::RequirementsData;

    return ret;
}

void LspEngine::HandleMessage(json const input_message) {
    if(input_message["method"] == "initialize") {
        l.LOG(LogLevel::DBUG, "Initialize Message received");

        Uri root_uri( input_message["params"]["rootUri"] );
        // TODO what if root uri is Null?
        ws.SetWorkspaceToInitialized(root_uri);
        RespondToInitialize(input_message);
    }
    else if (input_message["method"] == "initialized") {
        l.LOG(LogLevel::DBUG, "Initialized Message received");
    }
    else if (input_message["method"] == "textDocument/definition") {
        l.LOG(LogLevel::DBUG, "Definition Request received");
        Uri current_file (input_message["params"]["textDocument"]["uri"]);
        IdentifierPosition link;
        link.line_number = input_message["params"]["position"]["line"];
        link.position_in_line = input_message["params"]["position"]["character"];
        IdentifierPosition target;
        std::string target_path;

        json response_message {
            { "id", input_message["id"] },
            { "result", "" }
        };

        if(ws.GetTargetOfLink(current_file.GetPath(), link, target, target_path )) {
            json location {
                {"uri", target_path},
                { "range", {
                    {"start", {
                        { "line", target.line_number },
                        { "character", target.position_in_line },
                    }},
                    { "end", {
                        { "line", target.line_number },
                        { "character", target.position_in_line + target.length },
                    }},
                }},
            };

            response_message = {
                { "id", input_message["id"] },
                { "result", location }
            };
        }

        SendMessageToClient(response_message);
    }
    else if (input_message["method"] == "textDocument/didOpen" ||
             input_message["method"] == "textDocument/didChange") {
        Uri current_file (input_message["params"]["textDocument"]["uri"]);
        std::string text;

        if(input_message["method"] == "textDocument/didOpen") {
            text = input_message["params"]["textDocument"]["text"];
        }
        else {
            text = input_message["params"]["contentChanges"][0]["text"];
        }

        if(current_file.IsRequirementsData() || current_file.IsRequirementsSpecification()) {
            l.LOG(LogLevel::DBUG, "REL document " + current_file.GetPath() + " has been opened or changed");
            ParseDocument(current_file, text);
        }
    }
}

void LspEngine::ParseDocument(Uri const& uri, std::string const& text) {
    FileReader f(text);
    FileTokenData &data = ws.GetFileHandler(uri);
    data.SetFileReader(f);
    data.token_list.clear();

    Lexer lex(l);
    lex.Read(data);

    json diag;
    Uri uri_of_file = uri;

    try {
        ws.ParseTokens(data);

        l.LOG(LogLevel::DBUG, "REL file has been parsed, no errors");
        diag = json::array();
    }
    catch(EnumUsedButNotDefinedException &e) {
        Token const t  = e.GetToken();
        uri_of_file = Uri::CreateFileUriFromPath(*t.GetFilename());
        l.LOG(LogLevel::ERROR, *t.GetFilename() + ": Line " + std::to_string(t.GetLineNumberOfToken()) + ", Pos " + std::to_string(t.GetPositionInLineOfToken()) + ":");
        l.LOG(LogLevel::ERROR, e.what());
        l.LOG(LogLevel::DBUG, "Preparing JSON diagnostic message");
        diag = CreateDiagnosticsFromException(e);
    }
    catch(ParseException &e) {
        Token const t  = e.GetToken();
        l.LOG(LogLevel::ERROR, *t.GetFilename() + ": Line " + std::to_string(t.GetLineNumberOfToken()) + ", Pos " + std::to_string(t.GetPositionInLineOfToken()) + ":");
        l.LOG(LogLevel::ERROR, e.what());
        l.LOG(LogLevel::DBUG, "Preparing JSON diagnostic message");
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
                    { "definitionProvider", true},
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

    l.LOG(LogLevel::DBUG, "Message sent: " + header + full_message.dump(4));
}
