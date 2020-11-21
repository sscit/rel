#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

#include "Logger.h"
#include "MsgBuffer.h"
#include "LspEngine.h"

using namespace nlohmann;

void ProcessCommandLine(int const argc, char const * const argv[], Logger &l) {
    if(argc > 1) {
        for(int i=1; i<argc; ++i) {
            std::string argument(argv[i]);

            if(argument.compare("-v") == 0)
                l.SetLogLevel(LogLevel::INFO);
            else if(argument.compare("-vv") == 0)
                l.SetLogLevel(LogLevel::DEBUG);
        }
    }
}

int main(int argc, char* argv[]) {
    Logger l("/tmp/relc_ls_log");
    ProcessCommandLine(argc, argv, l);

    MsgBuffer message(l);
    LspEngine lsp(l);
    char c;

    l.LOG(LogLevel::DEBUG, "starting processing");
    while (std::cin.get(c)) {
        message.AddChar(c);
        
        if(message.IsMessageReady())
        {
            l.LOG(LogLevel::DEBUG, "Message received:");
            l.LOG(LogLevel::DEBUG, message.GetMessage());

            lsp.HandleMessage(json::parse(message.GetMessage()));

            message.Clear();
        }
    }
}