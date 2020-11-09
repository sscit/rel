#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

#include "Logger.h"
#include "MsgBuffer.h"
#include "LspEngine.h"

using namespace nlohmann;

int main(int argc, char* argv[]) {
    std::ofstream myfile;
    Logger l("/tmp/relc_ls_log");
    l.SetLogLevel(LogLevel::DEBUG);

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

    myfile.close();
}