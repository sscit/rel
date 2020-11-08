#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

#include "Logger.h"
#include "MsgBuffer.h"

using namespace nlohmann;

int main(int argc, char* argv[]) {
    std::ofstream myfile;
    Logger l("/tmp/relc_ls_log");
    l.SetLogLevel(LogLevel::DEBUG);
    
    MsgBuffer message(l);
    char c;
    l.LOG(LogLevel::DEBUG, "starting processing");
    while (std::cin.get(c)) {
        message.AddChar(c);
        
        if(message.IsMessageReady())
        {
            l.LOG(LogLevel::DEBUG, "MEssage is successfully read. Content:");
            l.LOG(LogLevel::DEBUG, message.GetMessage());
            json msg = json::parse(message.GetMessage());

            /*
            next step:
            - build a LspEngine which handles the protocol
            - it gets the json message and depending on its contents, it prepares
              the result
            - create a class that handles the state of the workspace, e.g. all
              files that have been opened and their content.
              the content of those files is used to feed the REL parser
            - it is necessary at the beginning, that all files are parsed, especially spec
            - then whenever a files changes, it is reparsed and added to the results of 
              the previous files

        }
    }

    myfile.close();
}