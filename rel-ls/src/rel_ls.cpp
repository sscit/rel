#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

#include "Logger.h"
#include "MsgBuffer.h"

int main(int argc, char* argv[]) {
    std::ofstream myfile;

    myfile.open ("example.txt");
    myfile << "Logging started " << std::endl;
    myfile.flush();

    MsgBuffer message;
    char c;

    while (std::cin.get(c)) {
        message.AddChar(c);
        
        if(message.IsMessageReady())
        {
            myfile << "message identified:" << std::endl;
            myfile << message.GetMessage() << std::endl;
            myfile.flush();
            message.Clear();
        }
    }

    myfile.close();
}